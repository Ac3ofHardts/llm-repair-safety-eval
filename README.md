# Does LLM security repair break safety rules?

**Cross-Domain Evaluation of LLM-Assisted Secure Code Generation: Preserving Safety Properties in Automotive and Avionics Firmware**
Evan C Hardt and Benjamin S Swartley, The University of Alabama (CS 544 Software Security, Fall 2026)

LLMs are increasingly used to fix security weaknesses in code, including embedded C for
vehicles and aircraft. That code also has to follow safety coding standards such as MISRA C,
which functional tests don't check. This repository holds everything for our study of whether
LLM security fixes preserve those rules: a benchmark of 10 verified seeded weaknesses in
real automotive (can-utils) and avionics (dump1090) code, the evaluation harness, all raw model
outputs and scores, and the paper source.

**Headline findings so far** (Milestone 2: 210 patches, 7 tasks; final results pending):
- 56% of patches fixed the seeded weakness, but **38% of those fixes introduced a new MISRA
  violation**. Every one was Rule 15.5 (single point of exit), from early-return guard clauses.
- Asking for security (vs. a plain review) didn't increase fixes (38 vs 36 of 70) but **nearly
  doubled safety violations** (30 vs 16 of 70).
- The security analyzers flagged **none** of the seeded weaknesses, and disagreed with the safety
  checker on 38% of patches, almost always accepting a patch the safety checker rejected.
- Weaknesses taken from **real upstream fixes** were repaired far less often than synthetic ones (10% vs 74%).
- 5 of 10 **human-written** upstream fixes also add Rule 15.5 violations.

## Repository map

```
benchmark/   the 10 tasks: seeds (patches), test drivers, tasks.json      -> benchmark/README.md
prompts/     T1 plain, T2 security, T3 analyzer-guided, T4 security + MISRA-aware
harness/     run.py (generate), eval.py (score), verify_seeds.py, semgrep rules
analysis/    summarize.py, code_export.py (qualitative coding), make_tables.py (paper tables)
results/     one directory per run tag; archive/m2 is frozen                -> results/README.md
paper/       main.tex + generated tables; milestones/ holds each submission
docs/        methodology.md, codebook.md, decisions.md (every design change, with reasons)
```

## Reproduce the paper's tables (no GPU needed)

Requires Linux with GCC (ASan/UBSan), Cppcheck with the MISRA addon, Python 3, git, and
`libncurses-dev`.

```bash
make setup        # clone + pin can-utils and dump1090, build dump1090 objects, install semgrep
make verify       # expect 10 x OK
make reproduce    # re-score the committed generations -> results -> paper/tables -> paper PDF
```

`make reproduce` reruns every compile, sanitizer, analyzer, and statistic from
`results/<tag>/generations.jsonl`. It doesn't need a model.

## Re-run the experiment (GPU)

Needs [Ollama](https://ollama.com) and a GPU with about 24 GB of memory (we used one RTX 4090).

```bash
ollama pull qwen2.5-coder:32b && ollama pull codestral:22b
make smoke                    # 1-rep sanity check
make run-base                 # canonical: temperature 0.2, T1-T4, 10 tasks, 5 reps
make run-t08                  # robustness: temperature 0.8
make eval TAG=base && make eval TAG=t08
make tables                   # regenerate paper/tables from results/base
```
Runs resume if interrupted. The full design is about 2 GPU-hours.

## Benchmark at a glance

| Domain | Text parsing | Binary / bit-level decoding |
|---|---|---|
| Automotive (can-utils) | A1, A2, **A3**, **A5** | A4 |
| Avionics (dump1090) | **V3**, V4 | V1, V2, V5 |

Bold = historical seed (restores a real upstream bug). Details: `benchmark/README.md`.

## Milestones

| Tag | What | Paper |
|---|---|---|
| `m2` | 7 tasks, T1–T3, 210 patches | `paper/milestones/m2/` |
| `m3-final` | 10 tasks, T1–T4, temperature robustness, qualitative coding | `paper/main.tex` |

The proposal and Milestone 1 draft are in `paper/milestones/m1/`. Every design change since
the proposal is logged in `docs/decisions.md`.

## Environment

GCC 15.2.0 · Cppcheck 2.19.0 · Semgrep 1.183.0 · Python 3.14.4 · Ollama 0.40.11 · Ubuntu ·
NVIDIA RTX 4090 (24 GB). Upstream pins: can-utils `834ceef`, dump1090 `0339a57` (v11.1).

## License

Our code and docs are MIT (see `LICENSE`). Seed patches, generations, and patched functions
modify can-utils (GPL-2.0-only OR BSD-3-Clause) and dump1090 (GPL-2.0-or-later) and are under
those licenses. The upstream code isn't redistributed; `setup.sh` fetches it.

## Citation

See `CITATION.cff`.
