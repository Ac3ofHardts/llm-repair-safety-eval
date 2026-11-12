#!/usr/bin/env python3
"""Export patches for qualitative coding.

Usage:
  python3 harness/code_export.py                 # tag "base"
  python3 harness/code_export.py --tag t08

Writes:
  results/coding_sheet[_<tag>].csv  one row per DISTINCT patch of interest, with heuristic
                                    labels and empty columns for your manual code and notes
  results/coding_cases[_<tag>].md   the same cases with diffs, for reading in Obsidian

A patch is "of interest" if the security and safety tools disagreed on it, it fixed the CWE but
introduced a safety violation, or it failed a historical seed or V2 (the hard tasks).
Heuristic labels are a starting point, not the final code: confirm or correct each one.
"""
import argparse
import csv
import difflib
import json
import re
from collections import defaultdict

from common import RESULTS, gens_path, results_path

CODES = ("guard_clause_return | restructured_condition | clamp_or_mask | deleted_logic | "
         "incomplete_bound | rewrite | no_change | other")


def truthy(v):
    return str(v).strip().lower() == "true"


def count_returns(code):
    return len(re.findall(r"\breturn\b", code or ""))


def heuristics(seeded, patch):
    sl, pl = (seeded or "").splitlines(), (patch or "").splitlines()
    diff = [d for d in difflib.unified_diff(sl, pl, lineterm="", n=0)
            if d[:1] in "+-" and d[:3] not in ("+++", "---")]
    added = [d[1:].strip() for d in diff if d.startswith("+")]
    removed = [d[1:].strip() for d in diff if d.startswith("-")]
    labels = []
    if count_returns(patch) > count_returns(seeded):
        labels.append("early_return_added")
    if any(re.match(r"if\s*\(", a) for a in added) and any("return" in a for a in added):
        labels.append("guard_clause")
    if any(re.search(r"\b(malloc|calloc|realloc|free)\s*\(", a) for a in added):
        labels.append("heap_added")
    if any(re.match(r"(while|for)\s*\(", a) for a in added):
        labels.append("loop_added")
    if any(re.search(r"&\s*0x[0-9a-fA-F]+|%\s*\w+|\bMIN\b|\bmin\(", a) for a in added):
        labels.append("mask_or_clamp")
    if len(removed) > 2 * max(1, len(added)):
        labels.append("mostly_deletions")
    ratio = difflib.SequenceMatcher(None, seeded or "", patch or "").ratio()
    if ratio < 0.6:
        labels.append("rewrite")
    if not diff:
        labels.append("no_change")
    return labels, len(added), len(removed), round(ratio, 3), "\n".join(diff)


def why(r):
    reasons = []
    if truthy(r.get("tool_disagree")):
        reasons.append("tool_disagree")
    if truthy(r["cwe_fixed"]) and truthy(r["safety_violation_introduced"]):
        reasons.append("fixed_but_unsafe")
    if r["seed_type"] == "historical" and not truthy(r["cwe_fixed"]):
        reasons.append("historical_not_fixed")
    if r["task"] == "V2" and not truthy(r["joint_success"]):
        reasons.append("V2_not_joint")
    return reasons


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--tag", default="base")
    args = ap.parse_args()
    suffix = "" if args.tag == "base" else f"_{args.tag}"

    gens = {}
    with open(gens_path(args.tag)) as f:
        for l in f:
            if l.strip():
                g = json.loads(l)
                gens[g["run_id"]] = g
    with open(results_path(args.tag)) as f:
        rows = [r for r in csv.DictReader(f) if truthy(r["extracted"])]

    groups = defaultdict(list)   # (model, task, technique, hash) -> rows
    for r in rows:
        if why(r):
            groups[(r["model"], r["task"], r["technique"], r.get("patch_hash") or r["run_id"])].append(r)

    out_rows, md = [], ["# Qualitative coding cases", "",
                        f"Codes: `{CODES}`", ""]
    for i, ((model, task, tech, h), rs) in enumerate(sorted(groups.items()), 1):
        r = rs[0]
        g = gens.get(r["run_id"], {})
        labels, n_add, n_rem, ratio, diff = heuristics(g.get("seeded_function"), g.get("final_function"))
        case = f"C{i:03d}"
        out_rows.append({
            "case": case, "model": model, "task": task, "technique": tech,
            "occurrences": len(rs), "run_ids": " ".join(x["run_id"] for x in rs),
            "reasons": " ".join(why(r)), "cwe_fixed": r["cwe_fixed"],
            "functional_pass": r["functional_pass"],
            "safety_violation_introduced": r["safety_violation_introduced"],
            "trigger_sanitizer": r["trigger_sanitizer"], "lines_added": n_add,
            "lines_removed": n_rem, "similarity_to_seeded": ratio,
            "heuristic_labels": " ".join(labels), "code": "", "notes": ""})
        md += [f"## {case}: {task} {tech} {model} (x{len(rs)})", "",
               f"- Reasons: {' '.join(why(r))}",
               f"- Fixed: {r['cwe_fixed']} | Functional: {r['functional_pass']} | "
               f"Safety violation: {r['safety_violation_introduced']} | "
               f"Sanitizer on trigger: {r['trigger_sanitizer']}",
               f"- Heuristic labels: {' '.join(labels) or '(none)'}",
               "- Code: ", "- Notes: ", "", "```diff", diff or "(identical to seeded function)",
               "```", ""]

    if not out_rows:
        raise SystemExit("no cases of interest")
    with open(RESULTS / f"coding_sheet{suffix}.csv", "w", newline="") as f:
        w = csv.DictWriter(f, fieldnames=list(out_rows[0].keys()))
        w.writeheader()
        w.writerows(out_rows)
    (RESULTS / f"coding_cases{suffix}.md").write_text("\n".join(md))
    total = sum(r["occurrences"] for r in out_rows)
    print(f"{len(out_rows)} distinct cases ({total} patches) -> results/coding_sheet{suffix}.csv "
          f"and coding_cases{suffix}.md")


if __name__ == "__main__":
    main()
