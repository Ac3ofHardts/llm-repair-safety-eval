#!/usr/bin/env python3
"""Evaluate generated patches on security, safety and functionality.

Usage:
  python3 harness/eval.py                 # evaluate everything not yet in results.csv
  python3 harness/eval.py --baseline-only # just print per-task baselines (original vs seeded)
  python3 harness/eval.py --tag t08       # evaluate results/t08/generations.jsonl -> results/t08/results.csv

Writes, under results/<tag>/: results.csv (one row per generation), baselines.json,
and the patched function for each run in patches/<run_id>.c
"""
import argparse
import csv
import difflib
import hashlib
import json
import shutil
from concurrent.futures import ProcessPoolExecutor
from collections import Counter

from common import (RESULTS, SAFETY_RULES, build_test, gens_path, is_archive, results_path, tag_dir, extract_function, function_line_range,
                    include_flags, load_config, make_workdir, misra_violations, normalize_c,
                    run_test, security_warnings, semgrep_loops, splice_function, warning_keys)

FIELDS = ["run_id", "tag", "temperature", "model", "task", "technique", "rep", "domain",
          "representation", "cwe", "seed_type", "patch_hash",
          "n_calls", "extracted", "build_ok", "functional_pass", "trigger_pass",
          "trigger_sanitizer", "sec_warnings", "new_sec_warnings", "cwe_fixed",
          *[f"misra_{r.replace('.', '_')}_new" for r in SAFETY_RULES], "loops_new",
          "safety_violation_introduced", "joint_success", "tool_sec_ok", "tool_safety_ok",
          "tool_disagree", "verbatim_upstream", "similarity_upstream", "lines_changed",
          "output_tokens", "gen_seconds", "error"]


def static_profile(cfg, task, wd):
    src = (wd / task["file"]).read_text()
    rng = function_line_range(src, task["function"])
    incs = include_flags(cfg, task, wd)
    sec = security_warnings(wd, task["file"], rng, incs)
    return {"sec": sec, "sec_keys": warning_keys(sec),
            "misra": misra_violations(wd, task["file"], rng, incs),
            "loops": Counter(w["rule"] for w in semgrep_loops(wd, task["file"], rng)),
            "function": extract_function(src, task["function"])}


def baselines(cfg):
    """Static profile of the ORIGINAL (upstream) and SEEDED function for each task."""
    out = {}
    for task in cfg["tasks"]:
        prof = {}
        for kind, seeded in (("original", False), ("seeded", True)):
            wd = make_workdir(cfg, task, seeded=seeded)
            prof[kind] = static_profile(cfg, task, wd)
            shutil.rmtree(wd, ignore_errors=True)
        out[task["id"]] = prof
    return out


def new_count(after: Counter, before: Counter):
    return sum((after - before).values())


def evaluate(cfg, rec, base, patch_dir):
    task = cfg["by_id"][rec["task"]]
    row = {k: "" for k in FIELDS}
    row.update(run_id=rec["run_id"], tag=rec.get("tag", "base"),
               temperature=rec.get("temperature", rec.get("options", {}).get("temperature", "")),
               model=rec["model"], task=task["id"], representation=task.get("representation", ""),
               technique=rec["technique"], rep=rec["rep"], domain=task["domain"],
               cwe=task["cwe"], seed_type=task["seed_type"], n_calls=len(rec.get("calls", [])),
               error=rec.get("error") or "")
    row["output_tokens"] = sum((c.get("eval_count") or 0) for c in rec.get("calls", []))
    row["gen_seconds"] = round(sum((c.get("wall_seconds") or 0) for c in rec.get("calls", [])), 1)
    fn = rec.get("final_function")
    row["extracted"] = fn is not None
    if fn is not None:
        row["patch_hash"] = hashlib.sha1(normalize_c(fn).encode()).hexdigest()[:12]
    if fn is None:
        for k in ("build_ok", "functional_pass", "trigger_pass", "cwe_fixed", "joint_success"):
            row[k] = False
        return row

    patch_dir.mkdir(exist_ok=True)
    (patch_dir / f"{rec['run_id'].replace(':', '_')}.c").write_text(fn + "\n")

    orig = base["original"]   # upstream function: used only for the contamination signal
    pre = base["seeded"]      # pre-patch function the model was given: baseline for "new" counts
    wd = make_workdir(cfg, task, seeded=True)
    try:
        seeded_src = (wd / task["file"]).read_text()
        (wd / task["file"]).write_text(splice_function(seeded_src, task["function"], fn))

        # build + dynamic checks
        ok, _ = build_test(cfg, task, wd, wd / "test_bin")
        row["build_ok"] = ok
        if ok:
            f = run_test(wd / "test_bin", "functional")
            t = run_test(wd / "test_bin", "trigger")
            row["functional_pass"] = f["pass"]
            row["trigger_pass"] = t["pass"]
            row["trigger_sanitizer"] = t["sanitizer"]
        else:
            row["functional_pass"] = row["trigger_pass"] = False

        # static checks on the patched function, compared with the PRE-PATCH (seeded) function
        prof = static_profile(cfg, task, wd)
        row["sec_warnings"] = len(prof["sec"])
        row["new_sec_warnings"] = new_count(prof["sec_keys"], pre["sec_keys"])
        safety_new = 0
        for r in SAFETY_RULES:
            n = max(0, prof["misra"].get(r, 0) - pre["misra"].get(r, 0))
            row[f"misra_{r.replace('.', '_')}_new"] = n
            safety_new += n
        row["loops_new"] = new_count(prof["loops"], pre["loops"])
        safety_new += row["loops_new"]

        row["safety_violation_introduced"] = safety_new > 0
        row["cwe_fixed"] = bool(ok and row["trigger_pass"] and row["new_sec_warnings"] == 0)
        row["joint_success"] = bool(row["cwe_fixed"] and row["functional_pass"]
                                    and not row["safety_violation_introduced"])
        row["tool_sec_ok"] = row["new_sec_warnings"] == 0
        row["tool_safety_ok"] = not row["safety_violation_introduced"]
        row["tool_disagree"] = row["tool_sec_ok"] != row["tool_safety_ok"]

        # contamination signal: how close is the patch to the upstream (fixed) function?
        up = orig["function"]
        row["verbatim_upstream"] = normalize_c(fn) == normalize_c(up)
        row["similarity_upstream"] = round(difflib.SequenceMatcher(
            None, normalize_c(fn), normalize_c(up)).ratio(), 3)
        diff = difflib.unified_diff(rec.get("seeded_function", "").splitlines(),
                                    fn.splitlines(), lineterm="", n=0)
        row["lines_changed"] = sum(1 for d in diff if d[:1] in "+-" and d[:3] not in ("+++", "---"))
    finally:
        shutil.rmtree(wd, ignore_errors=True)
    return row


def migrate(rows, out, gens, cfg):
    """Rewrite a results file from an older harness version with the current columns."""
    funcs = {}
    try:
        with open(gens) as f:
            for l in f:
                if l.strip():
                    r = json.loads(l)
                    funcs[r["run_id"]] = (r.get("final_function"), r.get("tag", "base"),
                                          r.get("options", {}).get("temperature", ""))
    except FileNotFoundError:
        pass
    for r in rows:
        fn, tag, temp = funcs.get(r["run_id"], (None, "base", ""))
        task = cfg["by_id"].get(r["task"], {})
        r.setdefault("tag", tag)
        r["tag"] = r["tag"] or tag
        r["temperature"] = r.get("temperature") or temp
        r["representation"] = r.get("representation") or task.get("representation", "")
        if not r.get("patch_hash") and fn:
            r["patch_hash"] = hashlib.sha1(normalize_c(fn).encode()).hexdigest()[:12]
    with open(out, "w", newline="") as f:
        w = csv.DictWriter(f, fieldnames=FIELDS, extrasaction="ignore", restval="")
        w.writeheader()
        w.writerows(rows)
    print(f"Migrated {len(rows)} existing rows in {out} to the current column layout")


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--tag", default="base")
    ap.add_argument("--gens", default=None)
    ap.add_argument("--out", default=None)
    ap.add_argument("--baseline-only", action="store_true")
    ap.add_argument("--jobs", type=int, default=4, help="parallel evaluations (CPU-bound)")
    args = ap.parse_args()
    if is_archive(args.tag):
        raise SystemExit("results/archive/ is a frozen snapshot; evaluate into a new tag instead")
    args.gens = args.gens or str(gens_path(args.tag))
    args.out = args.out or str(results_path(args.tag))
    cfg = load_config()

    print("Computing baselines (original vs seeded) ...")
    base = baselines(cfg)
    summary = {tid: {k: {"sec_warnings": [f"{w['tool']}:{w['rule']} L{w['line']}" for w in v["sec"]],
                         "misra": dict(v["misra"]), "loops": dict(v["loops"])}
                     for k, v in prof.items()} for tid, prof in base.items()}
    for tid, prof in base.items():
        up_new = {r: prof["original"]["misra"].get(r, 0) - prof["seeded"]["misra"].get(r, 0)
                  for r in SAFETY_RULES}
        up_new = {r: n for r, n in up_new.items() if n > 0}
        summary[tid]["upstream_fix_introduces_misra"] = up_new
        print(f"  {tid}: sec warnings seeded={len(prof['seeded']['sec'])} "
              f"original={len(prof['original']['sec'])}; MISRA seeded={dict(prof['seeded']['misra'])} "
              f"original={dict(prof['original']['misra'])}; upstream fix introduces {up_new or 'none'}")
    (tag_dir(args.tag) / "baselines.json").write_text(json.dumps(summary, indent=2))
    if args.baseline_only:
        return

    done = set()
    try:
        with open(args.out) as f:
            rd = csv.DictReader(f)
            old_rows = list(rd)
            old_fields = rd.fieldnames or []
        done = {r["run_id"] for r in old_rows}
        if old_fields != FIELDS:
            migrate(old_rows, args.out, args.gens, cfg)
    except FileNotFoundError:
        pass
    recs = []
    with open(args.gens) as f:
        recs = [json.loads(l) for l in f if l.strip()]
    todo = [r for r in recs if r["run_id"] not in done]
    print(f"{len(recs)} generations, {len(todo)} to evaluate")

    new_file = not done
    with open(args.out, "a", newline="") as f:
        w = csv.DictWriter(f, fieldnames=FIELDS)
        if new_file:
            w.writeheader()
        with ProcessPoolExecutor(max_workers=args.jobs) as pool:
            futs = [pool.submit(evaluate, cfg, rec, base[rec["task"]],
                                 tag_dir(args.tag) / "patches") for rec in todo]
            rows = (fut.result() for fut in futs)
            for k, (rec, row) in enumerate(zip(todo, rows), 1):
                w.writerow(row)
                f.flush()
                print(f"[{k}/{len(todo)}] {rec['run_id']}: build={row['build_ok']} "
                      f"func={row['functional_pass']} fixed={row['cwe_fixed']} "
                      f"safety_new={row['safety_violation_introduced']} joint={row['joint_success']}",
                      flush=True)


if __name__ == "__main__":
    main()
