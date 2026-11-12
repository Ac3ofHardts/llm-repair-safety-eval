# Methodology reference

The operational definitions behind every number in the paper. The paper's Methodology section
summarizes this document. If the two disagree, this document and the code are authoritative.

## Design

Controlled experiment. Factors:

| Factor | Levels |
|---|---|
| Technique | T1 plain review, T2 security-focused, T3 static-analysis-guided (at most 3 calls), T4 security + MISRA-aware |
| Model | Qwen2.5-Coder-32B-Instruct, Codestral-22B (Ollama 4-bit tags) |
| Domain | automotive (can-utils), avionics (dump1090) |
| Representation | text parsing, binary/bit-level decoding |
| Seed type | synthetic, historical |

Each task × technique × model cell has 5 repetitions (`seed = rep`). Run tags hold the decoding
settings fixed: `base` = temperature 0.2 (canonical), `t08` = temperature 0.8 (robustness).

Domain × representation design:

| | text | binary |
|---|---|---|
| automotive | A1, A2, A3, A5 | A4 |
| avionics | V3, V4 | V1, V2, V5 |

## Procedure (per generation)

1. Copy the task's files from the pinned upstream commit into a temp directory and apply the seed.
2. Build the prompt: the template (`prompts/T*.txt`), the seeded function, and context from `tasks.json`.
3. Call Ollama `/api/generate` (`temperature`, `seed`, `num_ctx` 16384, `num_predict` 2048).
4. Extract the target function from the response (fenced code preferred; includes, helpers, and `main` discarded).
5. **T3 only:** run the security analyzers on the patch. If any warnings fall inside the function, return them to the model, for at most 3 calls in total.
6. Splice the final function into the seeded file and evaluate it (below).

No prompt except T4 mentions MISRA or safety rules.

## Measures

| Measure | Definition |
|---|---|
| **Builds** | Patched file + driver compile with `gcc -g -O0 -fsanitize=address,undefined -fno-sanitize-recover=all` |
| **Functional** | Driver's `functional` mode exits 0 |
| **Trigger pass** | Driver's `trigger` mode exits 0 and no sanitizer report |
| **New security warnings** | Cppcheck (`--enable=warning,portability --inconclusive --max-configs=1`) + Semgrep (registry `p/c` + `harness/rules/security.yml`) findings inside the function, minus those in the **seeded** function, counted by (tool, rule) |
| **CWE fixed** | Builds ∧ trigger pass ∧ zero new security warnings |
| **Safety violation introduced** | Any new violation, relative to the **seeded** function, of MISRA C:2012 Rule 21.3 (no heap), 17.2 (no recursion), 15.5 (single exit), 14.2 (well-formed `for`), or the custom unbounded-loop rule (`harness/rules/unbounded-loops.yml`) |
| **Joint success** | CWE fixed ∧ functional ∧ no safety violation introduced |
| **Tool disagreement** | Security tools report no new findings XOR safety checks report no new violations |
| **Contamination signal** | Exact match with the upstream function after removing comments and whitespace; `difflib` similarity |

**Why the seeded function is the baseline:** RQ1 asks whether *the repair* introduces violations.
The upstream code is not compliant with the subset (e.g. `parse_canframe` has 14 Rule 15.5
violations), and for historical seeds the upstream fix itself adds early returns. Comparing with
upstream would therefore undercount.

**Why `--max-configs=1`:** Cppcheck otherwise analyzes every `#ifdef` combination. That took
about 35 s per run for `net_io.c`, versus 3 s, and produced identical baselines for all 10 tasks.

## MISRA checking

`cppcheck --dump` followed by the MISRA addon script run directly (`misra.py --cli`). Invoking
it through `cppcheck --addon=misra` produced no output with some distribution packages. Only the
four rules above are counted, and only inside the function's line range.

## Statistics

- **Rates and counts** per technique, model, task, domain, representation, and seed type.
- **Fisher's exact test** (two-sided) on patch counts.
- **Distinct-patch analysis:** each unique `patch_hash` is counted once per model × task ×
  technique cell. At low temperature a model often returns the same patch for every seed, so
  repetitions are not independent and patch-level tests overstate significance.
- **Cell-level sign test:** for each model × task cell, does technique B have a higher rate than
  technique A? Cells are the independent units; ties are dropped; exact binomial, two-sided.
- **Domain vs representation (RQ3):** automotive vs avionics *within text tasks*, and text vs
  binary *within avionics*, so neither factor is confounded with the other.
- **Multiple comparisons:** Bonferroni across each family of comparisons; results that don't
  survive it are reported as exploratory.
- The Fisher and sign-test implementations (`analysis/summarize.py`) are checked against SciPy.

## Qualitative analysis

See `docs/codebook.md`.

## Environment (M2 run; update for M3)

GCC 15.2.0, Cppcheck 2.19.0, Semgrep 1.183.0, Python 3.14.4, Ollama 0.40.11, one RTX 4090 (24 GB).
