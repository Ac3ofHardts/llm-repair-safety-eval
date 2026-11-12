#!/usr/bin/env python3
"""Verify every seed: original must pass both modes; seeded must fail the trigger.

Usage: python3 harness/verify_seeds.py [TASK_ID ...]
Writes results/seed_verification.csv
"""
import csv
import shutil
import sys

from common import RESULTS, build_test, load_config, make_workdir, run_test


def check(cfg, task, seeded):
    wd = make_workdir(cfg, task, seeded=seeded)
    binp = wd / "test_bin"
    ok, err = build_test(cfg, task, wd, binp)
    if not ok:
        shutil.rmtree(wd, ignore_errors=True)
        return {"build": False, "functional": None, "trigger": None, "note": err[-300:]}
    f = run_test(binp, "functional")
    t = run_test(binp, "trigger")
    shutil.rmtree(wd, ignore_errors=True)
    how = "sanitizer" if t["sanitizer"] else ("crash" if t["crash"] else ("check" if not t["pass"] else ""))
    return {"build": True, "functional": f["pass"], "trigger": t["pass"], "note": how}


def main():
    cfg = load_config()
    ids = sys.argv[1:] or [t["id"] for t in cfg["tasks"]]
    RESULTS.mkdir(exist_ok=True)
    rows, all_ok = [], True
    print(f"{'task':5} {'orig build/func/trig':22} {'seeded build/func/trig':24} verdict")
    for tid in ids:
        task = cfg["by_id"][tid]
        o = check(cfg, task, seeded=False)
        s = check(cfg, task, seeded=True)
        good = o["build"] and o["functional"] and o["trigger"] and s["build"] and s["trigger"] is False
        all_ok &= bool(good)
        fmt = lambda r: f"{r['build']}/{r['functional']}/{r['trigger']}"
        print(f"{tid:5} {fmt(o):22} {fmt(s):24} {'OK (' + s['note'] + ')' if good else 'PROBLEM'}")
        if not good:
            print(f"      orig note: {o['note']}\n      seeded note: {s['note']}")
        rows.append({"task": tid, "orig_build": o["build"], "orig_functional": o["functional"],
                     "orig_trigger": o["trigger"], "seeded_build": s["build"],
                     "seeded_functional": s["functional"], "seeded_trigger": s["trigger"],
                     "seeded_detected_by": s["note"], "verified": good})
    with open(RESULTS / "seed_verification.csv", "w", newline="") as f:
        w = csv.DictWriter(f, fieldnames=list(rows[0].keys()))
        w.writeheader()
        w.writerows(rows)
    sys.exit(0 if all_ok else 1)


if __name__ == "__main__":
    main()
