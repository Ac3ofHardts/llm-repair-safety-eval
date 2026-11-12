# Decision log

Every change to the study design, newest first. The paper's "Deviations from the plan"
paragraph summarizes this file.

## Milestone 3 (2026-11)

| Decision | Reason |
|---|---|
| Add V3 (historical, dump1090 `65cd5fe`), V4, V5 | M2's domain comparison had only 2 avionics tasks, both binary decoding, so domain was confounded with task and representation. The split is now 5 vs 5, with text and binary tasks in avionics. |
| Add a `representation` factor (text / binary) | Lets RQ3 separate "automotive vs avionics" from "text vs binary". |
| Add T4 (security + MISRA-aware prompt) | Tests whether the Rule 15.5 conflict disappears when the requirement is stated, and at what cost to the fix rate. |
| Add a temperature-0.8 run (`t08`) | At 0.2, Qwen often returned identical patches across seeds; `t08` is a robustness check with genuinely different samples. |
| Distinct-patch analysis + cell-level sign test | Identical repeats are not independent samples, so patch-level Fisher p-values are optimistic. |
| Cppcheck `--max-configs=1` | `net_io.c` analysis took 35 s vs 3 s; all 10 baselines were identical both ways. |
| Re-score all M2 generations with the M3 harness | Every row in `results/base/` then uses the same analysis setup. M2 numbers stay frozen in `results/archive/m2/`. |
| Repository restructure (`benchmark/`, `analysis/`, `paper/`, `docs/`, per-tag results) | One repo reproduces the whole paper; tables are generated, never hand-typed. |

## Milestone 2 (2026-11-03)

| Decision | Reason |
|---|---|
| can-utils pin v2025.01 → `master` `834ceef` | Two relevant upstream security fixes landed after the last release. |
| dump1090 pin v10.2 → v11.1 `0339a57` | v11.1 is the latest release. |
| A3 synthetic → historical (reverse of `cbbad5e`) | A real out-of-bounds read fix published after both models' training data was collected. |
| V1/V2 moved from `mode_s.c` to `cpr.c` (CWE-369, CWE-20) | The message decoder depends on global state, CRC tables, and scoring; `cpr.c` is self-contained and has upstream tests. |
| Inference llama.cpp → Ollama | Already deployed on the GPU host; same backend. |
| Safety baseline: upstream → pre-patch (seeded) function | Matches the RQ1 definition; avoids undercounting for historical seeds. |
| MISRA addon run directly on a Cppcheck dump | `cppcheck --addon=misra` produced no output with the distribution package. |
| Build can-utils against its bundled `include/` | CAN XL definitions are newer than the host's kernel headers. |

## Milestone 1 (2026-10-13)

| Decision | Reason |
|---|---|
| Two local open-weight models instead of a proprietary API model | All inference runs on one RTX 4090: reproducible weights, no data leaves the machine, no per-call cost. |
| A fixed MISRA C:2012 subset (21.3, 17.2, 15.5, 14.2) plus a loop rule | Free checkers cover only part of the standard; a fixed, explicit subset is reportable. |
| No prompt mentions MISRA (T1–T3) | Measures whether safety compliance is preserved *incidentally*, the way developers actually ask for fixes. |
