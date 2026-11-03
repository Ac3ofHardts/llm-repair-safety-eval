#!/usr/bin/env python3
"""Aggregate results/results.csv into paper-ready tables.

Usage: python3 harness/summarize.py [--csv results/results.csv]
Writes results/summary.md (Markdown, for Obsidian) and results/summary_tables.tex (LaTeX rows).
"""
import argparse
import csv
from collections import defaultdict
from math import comb

from common import RESULTS

METRICS = [("extracted", "Extracted"), ("build_ok", "Builds"), ("functional_pass", "Functional"),
           ("cwe_fixed", "CWE fixed"), ("safety_violation_introduced", "Safety viol. introduced"),
           ("joint_success", "Joint success"), ("tool_disagree", "Tool disagreement")]


def truthy(v):
    return str(v).strip().lower() == "true"


def rate(rows, key):
    if not rows:
        return None
    return sum(truthy(r[key]) for r in rows) / len(rows)


def pct(x):
    return "--" if x is None else f"{100 * x:.0f}%"


def fisher_two_sided(a, b, c, d):
    """Two-sided Fisher's exact test for [[a, b], [c, d]]."""
    n1, n2, k = a + b, c + d, a + c
    n = n1 + n2

    def p(x):
        return comb(n1, x) * comb(n2, k - x) / comb(n, k)

    p_obs = p(a)
    lo, hi = max(0, k - n2), min(k, n1)
    return min(1.0, sum(p(x) for x in range(lo, hi + 1) if p(x) <= p_obs + 1e-12))


def table(rows, group_keys, title):
    groups = defaultdict(list)
    for r in rows:
        groups[tuple(r[k] for k in group_keys)].append(r)
    hdr = [*group_keys, "N", *[m[1] for m in METRICS]]
    md = [f"### {title}", "", "| " + " | ".join(hdr) + " |", "|" + "---|" * len(hdr)]
    tex = [f"% {title}"]
    for g in sorted(groups):
        rs = groups[g]
        vals = [pct(rate(rs, m[0])) for m in METRICS]
        md.append("| " + " | ".join([*g, str(len(rs)), *vals]) + " |")
        tex.append(" & ".join([*g, str(len(rs)), *[v.replace('%', '\\%') for v in vals]]) + " \\\\")
    return "\n".join(md) + "\n", "\n".join(tex) + "\n"


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--csv", default=str(RESULTS / "results.csv"))
    args = ap.parse_args()
    with open(args.csv) as f:
        rows = [r for r in csv.DictReader(f) if r["model"] != "fake"]
    if not rows:
        raise SystemExit("no rows")

    md_parts, tex_parts = [f"# Results summary ({len(rows)} patches)\n"], []
    for keys, title in [(["technique", "domain"], "By technique and domain (RQ1, RQ3)"),
                        (["model", "technique"], "By model and technique"),
                        (["task", "technique"], "By task and technique"),
                        (["seed_type"], "By seed type (synthetic vs historical)")]:
        m, t = table(rows, keys, title)
        md_parts.append(m)
        tex_parts.append(t)

    # RQ2: disagreement breakdown
    sec_ok_safe_bad = sum(truthy(r["tool_sec_ok"]) and not truthy(r["tool_safety_ok"]) for r in rows
                          if truthy(r["extracted"]))
    safe_ok_sec_bad = sum(truthy(r["tool_safety_ok"]) and not truthy(r["tool_sec_ok"]) for r in rows
                          if truthy(r["extracted"]))
    fixed_but_unsafe = sum(truthy(r["cwe_fixed"]) and truthy(r["safety_violation_introduced"])
                           for r in rows)
    md_parts.append("### RQ2: security vs safety tool disagreement\n")
    md_parts.append(f"- Security tools OK, safety checker flags new violations: {sec_ok_safe_bad}")
    md_parts.append(f"- Safety checker OK, security tools flag new warnings: {safe_ok_sec_bad}")
    md_parts.append(f"- CWE actually fixed (trigger passes) but safety violation introduced: "
                    f"{fixed_but_unsafe}\n")

    # which MISRA rules get introduced
    rule_cols = [c for c in rows[0] if c.startswith("misra_") and c.endswith("_new")] + ["loops_new"]
    md_parts.append("### Introduced safety violations by rule\n")
    md_parts.append("| Rule | Patches introducing it | Total new violations |\n|---|---|---|")
    for c in rule_cols:
        vals = [int(r[c] or 0) for r in rows]
        md_parts.append(f"| {c.replace('misra_', '').replace('_new', '').replace('_', '.')} | "
                        f"{sum(v > 0 for v in vals)} | {sum(vals)} |")
    md_parts.append("")

    # RQ3: Fisher's exact test, per technique, safety_violation_introduced by domain
    md_parts.append("### RQ3: domain comparison (Fisher's exact, two-sided)\n")
    md_parts.append("| Technique | Metric | Automotive | Avionics | p |\n|---|---|---|---|---|")
    for tech in sorted({r["technique"] for r in rows}):
        for key, label in [("safety_violation_introduced", "Safety viol."), ("cwe_fixed", "CWE fixed"),
                           ("joint_success", "Joint")]:
            au = [r for r in rows if r["technique"] == tech and r["domain"] == "automotive"]
            av = [r for r in rows if r["technique"] == tech and r["domain"] == "avionics"]
            if not au or not av:
                continue
            a = sum(truthy(r[key]) for r in au)
            c = sum(truthy(r[key]) for r in av)
            p = fisher_two_sided(a, len(au) - a, c, len(av) - c)
            md_parts.append(f"| {tech} | {label} | {a}/{len(au)} | {c}/{len(av)} | {p:.3f} |")
    md_parts.append("")

    # contamination
    md_parts.append("### Contamination signal (similarity to upstream fixed function)\n")
    for st in sorted({r["seed_type"] for r in rows}):
        rs = [r for r in rows if r["seed_type"] == st and r["similarity_upstream"]]
        if rs:
            verb = sum(truthy(r["verbatim_upstream"]) for r in rs)
            mean = sum(float(r["similarity_upstream"]) for r in rs) / len(rs)
            md_parts.append(f"- {st}: {verb}/{len(rs)} verbatim matches, mean similarity {mean:.2f}")
    md_parts.append("")

    toks = sum(int(r["output_tokens"] or 0) for r in rows)
    secs = sum(float(r["gen_seconds"] or 0) for r in rows)
    md_parts.append(f"Generation cost: {toks} output tokens, {secs / 60:.1f} GPU-minutes total.\n")

    (RESULTS / "summary.md").write_text("\n".join(md_parts))
    (RESULTS / "summary_tables.tex").write_text("\n".join(tex_parts))
    print("\n".join(md_parts))


if __name__ == "__main__":
    main()
