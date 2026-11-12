# Milestone 2 results (frozen)

These are the results reported in the Milestone 2 paper (`paper/milestones/m2/main.tex`).
They are kept unchanged so the M2 submission stays reproducible. **Do not edit**; the
harness refuses to write here.

| | |
|---|---|
| Repository state | tag `m2` (commit `3ea1d46`) |
| Design | 7 tasks (A1–A5, V1, V2) × T1–T3 × 5 reps × 2 models = 210 patches |
| Models | `qwen2.5-coder:32b` (`b92d6a0bd47e`), `codestral:22b` (`0898a8b286d5`), 4-bit, Ollama 0.40.11 |
| Decoding | temperature 0.2, seed = rep, `num_ctx` 16384, `num_predict` 2048 |
| Tools | GCC 15.2.0, Cppcheck 2.19.0, Semgrep 1.183.0 (registry `p/c` + local rules), Python 3.14.4 |
| Hardware | 1 × NVIDIA RTX 4090 (24 GB) |
| GPU time | 35.2 minutes, 103,196 output tokens |

## Files

`generations.jsonl`, `results.csv`, `baselines.json`, `seed_verification.csv`, `summary.md`,
`summary_tables.tex`.

`patches/` was not committed at M2 (the old `.gitignore` excluded it). Every patch is
recoverable from `final_function` in `generations.jsonl`.

## Why superseded

The canonical results are now `results/base/`, produced by the M3 harness. Differences:

- 3 more tasks (V3–V5) and the T4 prompt
- Cppcheck runs with `--max-configs=1` (all baselines identical; much faster)
- extra result columns: `tag`, `temperature`, `representation`, `patch_hash`

`results/base/` re-scores these same 210 generations, so their verdicts can be compared one to one.

## Reproduce the M2 tables

```bash
python3 analysis/make_tables.py --tag archive/m2 --out /tmp/m2-tables
python3 analysis/summarize.py --tag archive/m2      # prints only; never writes here
```
