#!/usr/bin/env python3
"""Generate patches with local Ollama models.

Example:
  python3 harness/run.py --models qwen2.5-coder:32b codestral:22b --reps 5
  python3 harness/run.py --models qwen2.5-coder:32b --tasks A1 --techniques T1 --reps 1   # smoke test

Each (model, task, technique, rep) is one record in results/generations.jsonl.
Re-running skips records that already exist (safe to resume after a crash).
"""
import argparse
import json
import shutil
import sys
import time
import urllib.request
from datetime import datetime, timezone

from common import (PROMPTS, RESULTS, build_context, extract_code_from_response,
                    extract_function, format_warnings, function_line_range, include_flags,
                    load_config, make_workdir, security_warnings, splice_function)

PROJECT = {"can-utils": "can-utils, the Linux SocketCAN userspace utilities (automotive CAN bus tooling)",
           "dump1090": "dump1090, an ADS-B / Mode S aircraft transponder message decoder"}
INPUT_SOURCE = {"automotive": "from CAN bus frames and user-supplied CAN frame strings",
                "avionics": "decoded from over-the-air ADS-B (Mode S) transmissions"}
MAX_T3_CALLS = 3


def ollama(host, model, prompt, system, options, timeout=900):
    body = json.dumps({"model": model, "prompt": prompt, "system": system, "stream": False,
                       "options": options, "keep_alive": "30m"}).encode()
    req = urllib.request.Request(f"{host}/api/generate", data=body,
                                 headers={"Content-Type": "application/json"})
    t0 = time.time()
    with urllib.request.urlopen(req, timeout=timeout) as r:
        data = json.loads(r.read())
    data["_wall_seconds"] = round(time.time() - t0, 2)
    return data


def model_digests(host):
    try:
        with urllib.request.urlopen(f"{host}/api/tags", timeout=10) as r:
            return {m["name"]: m.get("digest", "") for m in json.loads(r.read()).get("models", [])}
    except Exception as e:  # noqa: BLE001
        sys.exit(f"Cannot reach Ollama at {host}: {e}")


def render(template, **kw):
    out = template
    for k, v in kw.items():
        out = out.replace("{" + k + "}", v)
    return out


def generate_one(cfg, task, technique, model, rep, args, templates, system):
    wd = make_workdir(cfg, task, seeded=True)
    try:
        src_path = wd / task["file"]
        seeded_src = src_path.read_text()
        seeded_fn = extract_function(seeded_src, task["function"])
        context = build_context(cfg, task, wd)
        base = dict(project=PROJECT[task["repo"]], input_source=INPUT_SOURCE[task["domain"]],
                    context=context, name=task["function"])
        options = {"temperature": args.temperature, "seed": rep, "num_ctx": args.num_ctx,
                   "num_predict": args.num_predict}
        incs = include_flags(cfg, task, wd)
        calls, current_fn, final_fn = [], seeded_fn, None
        n_calls = MAX_T3_CALLS if technique == "T3" else 1

        for i in range(n_calls):
            if technique == "T3":
                src_now = src_path.read_text()
                rng = function_line_range(src_now, task["function"])
                warns = security_warnings(wd, task["file"], rng, incs)
                if i > 0 and not warns:
                    break  # analyzer is satisfied; stop the feedback loop
                wtext = format_warnings(warns, rng) if warns else \
                    "- (no warnings were reported)"
                prompt = render(templates["T3"], function=current_fn, warnings=wtext, **base)
            else:
                warns = []
                prompt = render(templates[technique], function=current_fn, **base)

            resp = ollama(args.host, model, prompt, system, options)
            text = resp.get("response", "")
            fn = extract_code_from_response(text, task["function"])
            calls.append({"round": i + 1, "prompt": prompt, "response": text,
                          "warnings_given": warns, "extracted": fn is not None,
                          "eval_count": resp.get("eval_count"),
                          "prompt_eval_count": resp.get("prompt_eval_count"),
                          "total_duration_ns": resp.get("total_duration"),
                          "wall_seconds": resp.get("_wall_seconds")})
            if fn is None:
                break  # keep last good function (if any) as the final answer
            final_fn = fn
            current_fn = fn
            src_path.write_text(splice_function(seeded_src, task["function"], fn))
        return {"options": options, "calls": calls, "final_function": final_fn,
                "seeded_function": seeded_fn}
    finally:
        shutil.rmtree(wd, ignore_errors=True)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--host", default="http://localhost:11434")
    ap.add_argument("--models", nargs="+", default=["qwen2.5-coder:32b", "codestral:22b"])
    ap.add_argument("--tasks", nargs="+", default=None)
    ap.add_argument("--techniques", nargs="+", default=["T1", "T2", "T3"])
    ap.add_argument("--reps", type=int, default=5)
    ap.add_argument("--temperature", type=float, default=0.2)
    ap.add_argument("--num-ctx", type=int, default=16384)
    ap.add_argument("--num-predict", type=int, default=2048)
    ap.add_argument("--out", default=str(RESULTS / "generations.jsonl"))
    args = ap.parse_args()

    cfg = load_config()
    task_ids = args.tasks or [t["id"] for t in cfg["tasks"]]
    templates = {t: (PROMPTS / f"{t}.txt").read_text() for t in ("T1", "T2", "T3")}
    system = (PROMPTS / "system.txt").read_text().strip()
    digests = model_digests(args.host)
    for m in args.models:
        if m not in digests:
            sys.exit(f"Model {m} not found in Ollama. Run: ollama pull {m}")

    RESULTS.mkdir(exist_ok=True)
    done = set()
    try:
        with open(args.out) as f:
            done = {json.loads(l)["run_id"] for l in f if l.strip()}
    except FileNotFoundError:
        pass

    jobs = [(m, t, tech, r) for m in args.models for t in task_ids
            for tech in args.techniques for r in range(1, args.reps + 1)]
    todo = [j for j in jobs if f"{j[0]}__{j[1]}__{j[2]}__r{j[3]}" not in done]
    print(f"{len(jobs)} jobs, {len(jobs) - len(todo)} already done, {len(todo)} to run")

    with open(args.out, "a") as out:
        for k, (model, tid, tech, rep) in enumerate(todo, 1):
            run_id = f"{model}__{tid}__{tech}__r{rep}"
            t0 = time.time()
            try:
                res = generate_one(cfg, cfg["by_id"][tid], tech, model, rep, args, templates, system)
                err = None
            except Exception as e:  # noqa: BLE001  keep going; record the failure
                res, err = {"calls": [], "final_function": None}, repr(e)
            rec = {"run_id": run_id, "model": model, "model_digest": digests.get(model, ""),
                   "task": tid, "technique": tech, "rep": rep, "error": err,
                   "timestamp": datetime.now(timezone.utc).isoformat(), **res}
            out.write(json.dumps(rec) + "\n")
            out.flush()
            status = "ok" if rec.get("final_function") else f"NO FUNCTION {err or ''}"
            print(f"[{k}/{len(todo)}] {run_id}: {status} "
                  f"({len(rec['calls'])} call(s), {time.time() - t0:.0f}s)", flush=True)


if __name__ == "__main__":
    main()
