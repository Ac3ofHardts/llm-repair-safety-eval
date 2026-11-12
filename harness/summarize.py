#!/usr/bin/env python3
"""Aggregate results into paper-ready tables.

Usage:
  python3 harness/summarize.py                 # results/results.csv (tag "base")
  python3 harness/summarize.py --tag t08       # results/results_t08.csv
  python3 harness/summarize.py --tasks A1 A2 A3 A4 A5 V1 V2   # restrict to a task subset

Writes results/summary[_<tag>].md (Markdown, for Obsidian) and summary_tables[_<tag>].tex.

Statistics:
  * Fisher's exact test on patch counts treats every repetition as independent. When a model
    returns the same patch for several seeds this overstates significance, so we also report
  * a DISTINCT-PATCH analysis (each unique normalized patch counted once per
    model x task x technique cell), and
  * a CELL-LEVEL sign test: for each model x task cell, does technique X have a higher rate
    than technique Y? Cells are the independent units; ties are dropped.
"""
import argparse
import csv
from collections import defaultdict
from math import comb

from common import RESULTS, results_path

METRICS = [("extracted", "Extracted"), ("build_ok", "Builds"), ("functional_pass", "Functional"),
           ("cwe_fixed", "CWE fixed"), ("safety_violation_introduced", "Safety viol. introduced"),
           ("joint_success", "Joint success"), ("tool_disagree", "Tool disagreement")]
KEY = {"fixed": "cwe_fixed", "safety": "safety_violation_introduced", "joint": "joint_success"}


def truthy(v):
    return str(v).strip().lower() == "true"


def rate(rows, key):
    return None if not rows else sum(truthy(r[key]) for r in rows) / len(rows)


def pct(x):
    return "--" if x is None else f"{100 * x:.0f}%"


def count(rows, key):
    return sum(truthy(r[key]) for r in rows)


def fisher_two_sided(a, b, c, d):
    """Two-sided Fisher's exact test for [[a, b], [c, d]]."""
    n1, n2, k = a + b, c + d, a + c
    n = n1 + n2
    if n == 0 or k == 0 or k == n:
        return 1.0

    def p(x):
        return comb(n1, x) * comb(n2, k - x) / comb(n, k)

    p_obs = p(a)
    lo, hi = max(0, k - n2), min(k, n1)
    return min(1.0, sum(p(x) for x in range(lo, hi + 1) if p(x) <= p_obs * (1 + 1e-9)))


def sign_test(pos, neg):
    """Two-sided exact binomial sign test."""
    n = pos + neg
    if n == 0:
        return 1.0
    k = min(pos, neg)
    return min(1.0, 2 * sum(comb(n, i) for i in range(k + 1)) / 2 ** n)


def distinct(rows):
    """One row per unique patch within each model x task x technique cell."""
    seen, out = set(), []
    for r in rows:
        if not truthy(r["extracted"]):
            continue
        k = (r["model"], r["task"], r["technique"], r.get("patch_hash") or r["run_id"])
        if k not in seen:
            seen.add(k)
            out.append(r)
    return out


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


def compare(rows, field, a, b, metrics=("fixed", "safety", "joint")):
    """Fisher's exact test comparing groups field==a vs field==b, on all and distinct patches."""
    lines = []
    for m in metrics:
        key = KEY[m]
        for label, rs in (("all", rows), ("distinct", distinct(rows))):
            ga = [r for r in rs if r[field] == a]
            gb = [r for r in rs if r[field] == b]
            if not ga or not gb:
                continue
            x, y = count(ga, key), count(gb, key)
            p = fisher_two_sided(x, len(ga) - x, y, len(gb) - y)
            lines.append(f"| {a} vs {b} | {m} | {label} | {x}/{len(ga)} | {y}/{len(gb)} | {p:.3g} |")
    return lines


def cell_sign_test(rows, tech_a, tech_b, metric):
    """Sign test over model x task cells: is rate(tech_b) > rate(tech_a)?"""
    key = KEY[metric]
    cells = defaultdict(lambda: defaultdict(list))
    for r in rows:
        cells[(r["model"], r["task"])][r["technique"]].append(r)
    pos = neg = ties = 0
    for c in cells.values():
        if tech_a in c and tech_b in c:
            ra, rb = rate(c[tech_a], key), rate(c[tech_b], key)
            if rb > ra:
                pos += 1
            elif rb < ra:
                neg += 1
            else:
                ties += 1
    return pos, neg, ties, sign_test(pos, neg)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--tag", default="base")
    ap.add_argument("--csv", default=None)
    ap.add_argument("--tasks", nargs="+", default=None)
    args = ap.parse_args()
    path = args.csv or str(results_path(args.tag))
    with open(path) as f:
        rows = [r for r in csv.DictReader(f) if r["model"] not in ("fake", "oracle", "mock:latest")]
    if args.tasks:
        rows = [r for r in rows if r["task"] in args.tasks]
    if not rows:
        raise SystemExit("no rows")
    has_repr = bool(rows[0].get("representation"))

    md, tex = [f"# Results summary: {path} ({len(rows)} patches)\n"], []
    groupings = [(["technique", "domain"], "By technique and domain"),
                 (["model", "technique"], "By model and technique"),
                 (["task", "technique"], "By task and technique"),
                 (["seed_type"], "By seed type (synthetic vs historical)")]
    if has_repr:
        groupings.insert(1, (["technique", "representation"], "By technique and representation"))
        groupings.insert(2, (["domain", "representation"], "By domain and representation"))
    for keys, title in groupings:
        m, t = table(rows, keys, title)
        md.append(m)
        tex.append(t)

    # duplicates
    ext = [r for r in rows if truthy(r["extracted"])]
    d = distinct(rows)
    md.append("### Duplicate patches\n")
    md.append(f"- {len(ext)} extracted patches, {len(d)} distinct within their model x task x technique cell "
              f"({len(ext) - len(d)} duplicates).")
    per_model = defaultdict(lambda: [0, 0])
    for r in ext:
        per_model[r["model"]][0] += 1
    for r in d:
        per_model[r["model"]][1] += 1
    for mdl, (n, u) in sorted(per_model.items()):
        md.append(f"- {mdl}: {u} distinct of {n}")
    md.append("")

    # technique comparisons
    techs = sorted({r["technique"] for r in rows})
    md.append("### Technique comparisons (Fisher's exact, two-sided)\n")
    md.append("| Comparison | Metric | Patches | A | B | p |\n|---|---|---|---|---|---|")
    for b in techs:
        if b != "T1":
            md += compare(rows, "technique", "T1", b)
    if "T4" in techs and "T2" in techs:
        md += compare(rows, "technique", "T2", "T4")
    md.append("")
    md.append("### Technique comparisons, cell-level sign test (model x task cells)\n")
    md.append("| Comparison | Metric | B higher | A higher | Ties | p |\n|---|---|---|---|---|---|")
    pairs = [("T1", b) for b in techs if b != "T1"] + ([("T2", "T4")] if {"T2", "T4"} <= set(techs) else [])
    for a, b in pairs:
        for m in ("fixed", "safety", "joint"):
            pos, neg, ties, p = cell_sign_test(rows, a, b, m)
            md.append(f"| {a} vs {b} | {m} | {pos} | {neg} | {ties} | {p:.3g} |")
    md.append("")

    # RQ2
    md.append("### RQ2: security vs safety tool disagreement\n")
    md.append(f"- Security tools OK, safety checker flags new violations: "
              f"{sum(truthy(r['tool_sec_ok']) and not truthy(r['tool_safety_ok']) for r in ext)}")
    md.append(f"- Safety checker OK, security tools flag new warnings: "
              f"{sum(truthy(r['tool_safety_ok']) and not truthy(r['tool_sec_ok']) for r in ext)}")
    md.append(f"- CWE actually fixed but safety violation introduced: "
              f"{sum(truthy(r['cwe_fixed']) and truthy(r['safety_violation_introduced']) for r in rows)}"
              f" of {count(rows, 'cwe_fixed')} fixes\n")

    rule_cols = [c for c in rows[0] if c.startswith("misra_") and c.endswith("_new")] + ["loops_new"]
    md.append("### Introduced safety violations by rule\n")
    md.append("| Rule | Patches introducing it | Total new violations |\n|---|---|---|")
    for c in rule_cols:
        vals = [int(r[c] or 0) for r in rows]
        md.append(f"| {c.replace('misra_', '').replace('_new', '').replace('_', '.')} | "
                  f"{sum(v > 0 for v in vals)} | {sum(vals)} |")
    md.append("")

    # RQ3: domain and representation
    md.append("### RQ3: domain and representation (Fisher's exact, two-sided)\n")
    md.append("| Comparison | Metric | Patches | A | B | p |\n|---|---|---|---|---|---|")
    md += compare(rows, "domain", "automotive", "avionics")
    if has_repr:
        md += compare(rows, "representation", "text", "binary")
        av = [r for r in rows if r["domain"] == "avionics"]
        md += [l.replace("| text vs binary", "| avionics: text vs binary") for l in
               compare(av, "representation", "text", "binary")]
        tx = [r for r in rows if r["representation"] == "text"]
        md += [l.replace("| automotive vs avionics", "| text only: automotive vs avionics") for l in
               compare(tx, "domain", "automotive", "avionics")]
    md.append("")
    md.append("| Technique | Metric | Automotive | Avionics | p |\n|---|---|---|---|---|")
    for tech in techs:
        for m in ("safety", "fixed", "joint"):
            key = KEY[m]
            au = [r for r in rows if r["technique"] == tech and r["domain"] == "automotive"]
            av = [r for r in rows if r["technique"] == tech and r["domain"] == "avionics"]
            if au and av:
                a, c = count(au, key), count(av, key)
                p = fisher_two_sided(a, len(au) - a, c, len(av) - c)
                md.append(f"| {tech} | {m} | {a}/{len(au)} | {c}/{len(av)} | {p:.3f} |")
    md.append("")

    md.append("### Contamination signal (similarity to upstream fixed function)\n")
    for st in sorted({r["seed_type"] for r in rows}):
        rs = [r for r in rows if r["seed_type"] == st and r["similarity_upstream"]]
        if rs:
            verb = sum(truthy(r["verbatim_upstream"]) for r in rs)
            mean = sum(float(r["similarity_upstream"]) for r in rs) / len(rs)
            md.append(f"- {st}: {verb}/{len(rs)} verbatim matches, mean similarity {mean:.2f}")
    md.append("")
    if "n_calls" in rows[0]:
        t3 = [r for r in rows if r["technique"] == "T3"]
        multi = sum(int(r["n_calls"] or 0) > 1 for r in t3)
        md.append(f"T3 runs with analyzer feedback (more than one call): {multi} of {len(t3)}.\n")
    toks = sum(int(r["output_tokens"] or 0) for r in rows)
    secs = sum(float(r["gen_seconds"] or 0) for r in rows)
    md.append(f"Generation cost: {toks} output tokens, {secs / 60:.1f} GPU-minutes total.\n")

    suffix = "" if args.tag == "base" and not args.csv else f"_{args.tag}"
    (RESULTS / f"summary{suffix}.md").write_text("\n".join(md))
    (RESULTS / f"summary_tables{suffix}.tex").write_text("\n".join(tex))
    print("\n".join(md))


if __name__ == "__main__":
    main()
