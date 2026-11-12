# Results

Each **run tag** is one combination of settings. Every tag has its own self-contained
directory, and tags are never pooled in one statistical test.

| Directory | Settings | Status |
|---|---|---|
| `base/` | temperature 0.2, T1–T4, 10 tasks, 5 reps, 2 models | **canonical**; the paper's main results |
| `t08/` | temperature 0.8, T1–T4 | robustness check |
| `archive/m2/` | the Milestone 2 run (7 tasks, T1–T3) | **frozen**; see `archive/m2/MANIFEST.md` |
| `coding/` | qualitative coding of `base` | manual codes + agreement |

`base/generations.jsonl` starts as a copy of the M2 generations, so the M3 run resumes on top of
them instead of regenerating the 210 M2 patches. `eval.py` then re-scores everything with the
current harness.

`run.py`, `eval.py`, and `summarize.py` refuse to write into `archive/`.

## Files in a tag directory

| File | Written by | Contents |
|---|---|---|
| `generations.jsonl` | `harness/run.py` | One JSON record per generation (schema below) |
| `results.csv` | `harness/eval.py` | One row per generation (columns below) |
| `baselines.json` | `harness/eval.py` | Static profile of each task's original and seeded function |
| `patches/<run_id>.c` | `harness/eval.py` | The extracted function from each generation |
| `summary.md`, `summary_tables.tex` | `analysis/summarize.py` | Aggregated tables and tests |
| `seed_verification.csv` | `harness/verify_seeds.py` | `base/` only |

## `generations.jsonl` record

| Field | Meaning |
|---|---|
| `run_id` | `[<tag>__]<model>__<task>__<technique>__r<rep>` (no prefix for `base`) |
| `tag`, `temperature` | Run settings |
| `model`, `model_digest` | Ollama model tag and digest |
| `task`, `technique`, `rep` | Design cell; `rep` is also the sampling seed |
| `options` | Exact Ollama options (temperature, seed, `num_ctx`, `num_predict`) |
| `calls[]` | Per model call: `prompt`, `response`, `warnings_given` (T3), `extracted`, token counts, timings |
| `seeded_function` | The function the model was given |
| `final_function` | The function extracted from the last successful call (`null` if none) |
| `error`, `timestamp` | Runner error (if any) and UTC time |

## `results.csv` columns

| Column | Meaning |
|---|---|
| `run_id`, `tag`, `temperature`, `model`, `task`, `technique`, `rep` | Identity |
| `domain`, `representation`, `cwe`, `seed_type` | Task attributes from `benchmark/tasks.json` |
| `patch_hash` | SHA-1 (12 hex) of the patch with comments and whitespace removed; identical patches share a hash |
| `n_calls` | Model calls used (T3 can use up to 3) |
| `extracted` | A function with the target name was recovered from the response |
| `build_ok` | Patched file + test driver compiled with ASan/UBSan |
| `functional_pass` | Functional test mode passed |
| `trigger_pass` | Trigger test mode passed with no sanitizer report |
| `trigger_sanitizer` | A sanitizer fired in trigger mode |
| `sec_warnings` | Cppcheck + Semgrep findings inside the patched function |
| `new_sec_warnings` | Findings not present in the pre-patch (seeded) function |
| `cwe_fixed` | `build_ok` and `trigger_pass` and `new_sec_warnings == 0` |
| `misra_21_3_new`, `misra_17_2_new`, `misra_15_5_new`, `misra_14_2_new` | New violations of each MISRA C:2012 rule, relative to the seeded function |
| `loops_new` | New findings from the custom unbounded-loop rule |
| `safety_violation_introduced` | Any of the five counts above is > 0 |
| `joint_success` | `cwe_fixed` and `functional_pass` and no safety violation introduced |
| `tool_sec_ok`, `tool_safety_ok`, `tool_disagree` | Static-tool verdicts (no new findings) and whether they disagree |
| `verbatim_upstream`, `similarity_upstream` | Exact match / sequence similarity to the upstream (fixed) function, a contamination signal |
| `lines_changed` | Changed lines relative to the seeded function |
| `output_tokens`, `gen_seconds` | Generation cost |
| `error` | Runner error, if any |

Precise definitions and the statistics are in `docs/methodology.md`.

## Licensing

`generations.jsonl` and `patches/` contain model-generated modifications of can-utils
(GPL-2.0-only OR BSD-3-Clause) and dump1090 (GPL-2.0-or-later) functions, and are covered by
those licenses, not the repository's MIT license.
