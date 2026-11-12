# Qualitative codebook

Used to code how a model changed a function, for the patches exported by `make coding`
(`results/coding/coding_sheet.csv`).

## Which patches are coded

A distinct patch is exported if **any** of these is true:
- the security tools and the safety checker disagreed on it;
- it fixed the CWE but introduced a safety violation;
- it is for a historical seed (A3, A5, V3) and did not fix the CWE;
- it is for V2 and did not achieve joint success.

Identical patches (same `patch_hash` within a model × task × technique cell) are coded once;
`occurrences` records how many generations produced the patch.

## Codes

Assign exactly **one primary code** per case, and put secondary observations in `notes`.

| Code | Definition | Example |
|---|---|---|
| `guard_clause_return` | Adds `if (bad) return …;` before the vulnerable operation. This is the Rule 15.5 pattern. | A4: `if (dlc > 15) return 0;` before the table lookup |
| `restructured_condition` | Extends an existing condition or loop bound; no new exit points | A2: `if (!len \|\| len % 2 \|\| …)` |
| `clamp_or_mask` | Bounds the value (mask, clamp, modulo) instead of rejecting it | A4: `dlc2len[dlc & 0x0F]` |
| `incomplete_bound` | Adds a check that doesn't cover the triggering input | A5: checks the first `snprintf` but not the second |
| `deleted_logic` | Removes required behavior (the failure mode Dai et al. describe) | drops the CAN XL branch |
| `rewrite` | Substantially rewrites the function | V2 (Qwen, T1): similarity 0.31 to upstream |
| `no_change` | Behaviorally the same as the seeded function | cosmetic edits only |
| `other` | None of the above; explain in `notes` | |

The `heuristic_labels` column (`early_return_added`, `guard_clause`, `heap_added`, `loop_added`,
`mask_or_clamp`, `mostly_deletions`, `rewrite`, `no_change`) is generated from the diff. It is
a starting point, not the code: confirm or override it.

## Procedure

1. Evan codes the automotive cases and Ben codes the avionics cases.
2. Draw 20 cases at random (fixed seed, recorded in `notes`). Both authors code them
   **independently**, without seeing the other's codes, into `results/coding/agreement.csv`.
3. Report percent agreement and Cohen's κ on those 20.
4. Resolve disagreements by discussion, and record the result in `resolved_code`.
5. Report the code × technique and code × seed type tables in the paper.
