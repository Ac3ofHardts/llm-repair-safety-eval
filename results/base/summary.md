# Results summary: results/base/results.csv (400 patches)

### By technique and domain

| technique | domain | N | Extracted | Builds | Functional | CWE fixed | Safety viol. introduced | Joint success | Tool disagreement |
|---|---|---|---|---|---|---|---|---|---|
| T1 | automotive | 50 | 98% | 98% | 80% | 60% | 32% | 38% | 32% |
| T1 | avionics | 50 | 94% | 92% | 56% | 20% | 20% | 2% | 20% |
| T2 | automotive | 50 | 98% | 90% | 76% | 66% | 40% | 40% | 46% |
| T2 | avionics | 50 | 90% | 90% | 68% | 30% | 62% | 8% | 62% |
| T3 | automotive | 50 | 94% | 92% | 70% | 64% | 46% | 36% | 46% |
| T3 | avionics | 50 | 88% | 88% | 66% | 32% | 34% | 12% | 34% |
| T4 | automotive | 50 | 92% | 92% | 70% | 62% | 44% | 22% | 46% |
| T4 | avionics | 50 | 86% | 86% | 54% | 20% | 20% | 10% | 20% |

### By technique and representation

| technique | representation | N | Extracted | Builds | Functional | CWE fixed | Safety viol. introduced | Joint success | Tool disagreement |
|---|---|---|---|---|---|---|---|---|---|
| T1 | binary | 40 | 95% | 92% | 62% | 50% | 40% | 12% | 40% |
| T1 | text | 60 | 97% | 97% | 72% | 33% | 17% | 25% | 17% |
| T2 | binary | 40 | 100% | 100% | 82% | 50% | 70% | 15% | 70% |
| T2 | text | 60 | 90% | 83% | 65% | 47% | 38% | 30% | 43% |
| T3 | binary | 40 | 100% | 100% | 78% | 65% | 60% | 18% | 60% |
| T3 | text | 60 | 85% | 83% | 62% | 37% | 27% | 28% | 27% |
| T4 | binary | 40 | 100% | 100% | 75% | 50% | 32% | 25% | 32% |
| T4 | text | 60 | 82% | 82% | 53% | 35% | 32% | 10% | 33% |

### By domain and representation

| domain | representation | N | Extracted | Builds | Functional | CWE fixed | Safety viol. introduced | Joint success | Tool disagreement |
|---|---|---|---|---|---|---|---|---|---|
| automotive | binary | 40 | 100% | 100% | 100% | 100% | 70% | 30% | 70% |
| automotive | text | 160 | 94% | 91% | 68% | 54% | 33% | 35% | 36% |
| avionics | binary | 120 | 98% | 98% | 66% | 38% | 44% | 13% | 44% |
| avionics | text | 80 | 76% | 76% | 54% | 6% | 19% | 0% | 19% |

### By model and technique

| model | technique | N | Extracted | Builds | Functional | CWE fixed | Safety viol. introduced | Joint success | Tool disagreement |
|---|---|---|---|---|---|---|---|---|---|
| codestral:22b | T1 | 50 | 92% | 90% | 66% | 40% | 40% | 12% | 40% |
| codestral:22b | T2 | 50 | 88% | 80% | 60% | 44% | 48% | 14% | 54% |
| codestral:22b | T3 | 50 | 82% | 80% | 56% | 40% | 36% | 10% | 36% |
| codestral:22b | T4 | 50 | 78% | 78% | 54% | 40% | 44% | 6% | 44% |
| qwen2.5-coder:32b | T1 | 50 | 100% | 100% | 70% | 40% | 12% | 28% | 12% |
| qwen2.5-coder:32b | T2 | 50 | 100% | 100% | 84% | 52% | 54% | 34% | 54% |
| qwen2.5-coder:32b | T3 | 50 | 100% | 100% | 80% | 56% | 44% | 38% | 44% |
| qwen2.5-coder:32b | T4 | 50 | 100% | 100% | 70% | 42% | 20% | 26% | 22% |

### By task and technique

| task | technique | N | Extracted | Builds | Functional | CWE fixed | Safety viol. introduced | Joint success | Tool disagreement |
|---|---|---|---|---|---|---|---|---|---|
| A1 | T1 | 10 | 100% | 100% | 100% | 100% | 50% | 50% | 50% |
| A1 | T2 | 10 | 100% | 100% | 100% | 100% | 50% | 50% | 50% |
| A1 | T3 | 10 | 100% | 100% | 90% | 100% | 50% | 50% | 50% |
| A1 | T4 | 10 | 100% | 100% | 100% | 100% | 100% | 0% | 100% |
| A2 | T1 | 10 | 100% | 100% | 100% | 100% | 0% | 100% | 0% |
| A2 | T2 | 10 | 100% | 100% | 100% | 100% | 0% | 100% | 0% |
| A2 | T3 | 10 | 100% | 100% | 90% | 90% | 10% | 90% | 10% |
| A2 | T4 | 10 | 100% | 100% | 80% | 80% | 50% | 30% | 60% |
| A3 | T1 | 10 | 90% | 90% | 0% | 0% | 0% | 0% | 0% |
| A3 | T2 | 10 | 100% | 60% | 0% | 0% | 20% | 0% | 50% |
| A3 | T3 | 10 | 100% | 90% | 0% | 0% | 40% | 0% | 40% |
| A3 | T4 | 10 | 80% | 80% | 0% | 0% | 0% | 0% | 0% |
| A4 | T1 | 10 | 100% | 100% | 100% | 100% | 60% | 40% | 60% |
| A4 | T2 | 10 | 100% | 100% | 100% | 100% | 80% | 20% | 80% |
| A4 | T3 | 10 | 100% | 100% | 100% | 100% | 90% | 10% | 90% |
| A4 | T4 | 10 | 100% | 100% | 100% | 100% | 50% | 50% | 50% |
| A5 | T1 | 10 | 100% | 100% | 100% | 0% | 50% | 0% | 50% |
| A5 | T2 | 10 | 90% | 90% | 80% | 30% | 50% | 30% | 50% |
| A5 | T3 | 10 | 70% | 70% | 70% | 30% | 40% | 30% | 40% |
| A5 | T4 | 10 | 80% | 80% | 70% | 30% | 20% | 30% | 20% |
| V1 | T1 | 10 | 80% | 80% | 10% | 10% | 0% | 10% | 0% |
| V1 | T2 | 10 | 100% | 100% | 40% | 40% | 10% | 40% | 10% |
| V1 | T3 | 10 | 100% | 100% | 60% | 60% | 0% | 60% | 0% |
| V1 | T4 | 10 | 100% | 100% | 0% | 0% | 0% | 0% | 0% |
| V2 | T1 | 10 | 100% | 100% | 50% | 50% | 0% | 0% | 0% |
| V2 | T2 | 10 | 100% | 100% | 90% | 10% | 90% | 0% | 90% |
| V2 | T3 | 10 | 100% | 100% | 50% | 50% | 50% | 0% | 50% |
| V2 | T4 | 10 | 100% | 100% | 100% | 0% | 30% | 0% | 30% |
| V3 | T1 | 10 | 100% | 100% | 50% | 0% | 0% | 0% | 0% |
| V3 | T2 | 10 | 70% | 70% | 50% | 50% | 60% | 0% | 60% |
| V3 | T3 | 10 | 70% | 70% | 70% | 0% | 0% | 0% | 0% |
| V3 | T4 | 10 | 60% | 60% | 40% | 0% | 0% | 0% | 0% |
| V4 | T1 | 10 | 90% | 90% | 80% | 0% | 0% | 0% | 0% |
| V4 | T2 | 10 | 80% | 80% | 60% | 0% | 50% | 0% | 50% |
| V4 | T3 | 10 | 70% | 70% | 50% | 0% | 20% | 0% | 20% |
| V4 | T4 | 10 | 70% | 70% | 30% | 0% | 20% | 0% | 20% |
| V5 | T1 | 10 | 100% | 90% | 90% | 40% | 100% | 0% | 100% |
| V5 | T2 | 10 | 100% | 100% | 100% | 50% | 100% | 0% | 100% |
| V5 | T3 | 10 | 100% | 100% | 100% | 50% | 100% | 0% | 100% |
| V5 | T4 | 10 | 100% | 100% | 100% | 100% | 50% | 50% | 50% |

### By seed type (synthetic vs historical)

| seed_type | N | Extracted | Builds | Functional | CWE fixed | Safety viol. introduced | Joint success | Tool disagreement |
|---|---|---|---|---|---|---|---|---|
| historical | 120 | 84% | 80% | 44% | 12% | 23% | 8% | 26% |
| synthetic | 280 | 96% | 96% | 78% | 58% | 43% | 27% | 44% |

### Duplicate patches

- 370 extracted patches, 187 distinct within their model x task x technique cell (183 duplicates).
- codestral:22b: 98 distinct of 170
- qwen2.5-coder:32b: 89 distinct of 200

### Technique comparisons (Fisher's exact, two-sided)

| Comparison | Metric | Patches | A | B | p |
|---|---|---|---|---|---|
| T1 vs T2 | fixed | all | 40/100 | 48/100 | 0.319 |
| T1 vs T2 | fixed | distinct | 12/45 | 20/51 | 0.278 |
| T1 vs T2 | safety | all | 26/100 | 51/100 | 0.000451 |
| T1 vs T2 | safety | distinct | 14/45 | 26/51 | 0.0627 |
| T1 vs T2 | joint | all | 20/100 | 24/100 | 0.609 |
| T1 vs T2 | joint | distinct | 5/45 | 9/51 | 0.401 |
| T1 vs T3 | fixed | all | 40/100 | 48/100 | 0.319 |
| T1 vs T3 | fixed | distinct | 12/45 | 18/41 | 0.116 |
| T1 vs T3 | safety | all | 26/100 | 40/100 | 0.0502 |
| T1 vs T3 | safety | distinct | 14/45 | 18/41 | 0.267 |
| T1 vs T3 | joint | all | 20/100 | 24/100 | 0.609 |
| T1 vs T3 | joint | distinct | 5/45 | 9/41 | 0.244 |
| T1 vs T4 | fixed | all | 40/100 | 41/100 | 1 |
| T1 vs T4 | fixed | distinct | 12/45 | 19/50 | 0.278 |
| T1 vs T4 | safety | all | 26/100 | 32/100 | 0.436 |
| T1 vs T4 | safety | distinct | 14/45 | 17/50 | 0.828 |
| T1 vs T4 | joint | all | 20/100 | 16/100 | 0.581 |
| T1 vs T4 | joint | distinct | 5/45 | 7/50 | 0.763 |
| T2 vs T4 | fixed | all | 48/100 | 41/100 | 0.393 |
| T2 vs T4 | fixed | distinct | 20/51 | 19/50 | 1 |
| T2 vs T4 | safety | all | 51/100 | 32/100 | 0.0096 |
| T2 vs T4 | safety | distinct | 26/51 | 17/50 | 0.108 |
| T2 vs T4 | joint | all | 24/100 | 16/100 | 0.216 |
| T2 vs T4 | joint | distinct | 9/51 | 7/50 | 0.786 |

### Technique comparisons, cell-level sign test (model x task cells)

| Comparison | Metric | B higher | A higher | Ties | p |
|---|---|---|---|---|---|
| T1 vs T2 | fixed | 5 | 1 | 14 | 0.219 |
| T1 vs T2 | safety | 10 | 1 | 9 | 0.0117 |
| T1 vs T2 | joint | 3 | 1 | 16 | 0.625 |
| T1 vs T3 | fixed | 3 | 1 | 16 | 0.625 |
| T1 vs T3 | safety | 6 | 1 | 13 | 0.125 |
| T1 vs T3 | joint | 2 | 2 | 16 | 1 |
| T1 vs T4 | fixed | 3 | 3 | 14 | 1 |
| T1 vs T4 | safety | 5 | 3 | 12 | 0.727 |
| T1 vs T4 | joint | 3 | 4 | 13 | 1 |
| T2 vs T4 | fixed | 2 | 6 | 12 | 0.289 |
| T2 vs T4 | safety | 3 | 11 | 6 | 0.0574 |
| T2 vs T4 | joint | 3 | 6 | 11 | 0.508 |

### RQ2: security vs safety tool disagreement

- Security tools OK, safety checker flags new violations: 148
- Safety checker OK, security tools flag new warnings: 5
- CWE actually fixed but safety violation introduced: 88 of 177 fixes

### Introduced safety violations by rule

| Rule | Patches introducing it | Total new violations |
|---|---|---|
| 21.3 | 0 | 0 |
| 17.2 | 0 | 0 |
| 15.5 | 146 | 211 |
| 14.2 | 0 | 0 |
| loops | 8 | 16 |

### RQ3: domain and representation (Fisher's exact, two-sided)

| Comparison | Metric | Patches | A | B | p |
|---|---|---|---|---|---|
| automotive vs avionics | fixed | all | 126/200 | 51/200 | 4.56e-14 |
| automotive vs avionics | fixed | distinct | 48/98 | 21/89 | 0.000448 |
| automotive vs avionics | safety | all | 81/200 | 68/200 | 0.215 |
| automotive vs avionics | safety | distinct | 46/98 | 29/89 | 0.0528 |
| automotive vs avionics | joint | all | 68/200 | 16/200 | 1.22e-10 |
| automotive vs avionics | joint | distinct | 22/98 | 8/89 | 0.016 |
| text vs binary | fixed | all | 91/240 | 86/160 | 0.00206 |
| text vs binary | fixed | distinct | 34/120 | 35/67 | 0.00154 |
| text vs binary | safety | all | 68/240 | 81/160 | 8.48e-06 |
| text vs binary | safety | distinct | 43/120 | 32/67 | 0.122 |
| text vs binary | joint | all | 56/240 | 28/160 | 0.17 |
| text vs binary | joint | distinct | 17/120 | 13/67 | 0.407 |
| avionics: text vs binary | fixed | all | 5/80 | 46/120 | 1.24e-07 |
| avionics: text vs binary | fixed | distinct | 3/39 | 18/50 | 0.00217 |
| avionics: text vs binary | safety | all | 15/80 | 53/120 | 0.00023 |
| avionics: text vs binary | safety | distinct | 9/39 | 20/50 | 0.113 |
| avionics: text vs binary | joint | all | 0/80 | 16/120 | 0.000252 |
| avionics: text vs binary | joint | distinct | 0/39 | 8/50 | 0.00847 |
| text only: automotive vs avionics | fixed | all | 86/160 | 5/80 | 2.58e-14 |
| text only: automotive vs avionics | fixed | distinct | 31/81 | 3/39 | 0.000421 |
| text only: automotive vs avionics | safety | all | 53/160 | 15/80 | 0.0226 |
| text only: automotive vs avionics | safety | distinct | 34/81 | 9/39 | 0.0665 |
| text only: automotive vs avionics | joint | all | 56/160 | 0/80 | 4.53e-12 |
| text only: automotive vs avionics | joint | distinct | 17/81 | 0/39 | 0.00128 |

| Technique | Metric | Automotive | Avionics | p |
|---|---|---|---|---|
| T1 | safety | 16/50 | 10/50 | 0.254 |
| T1 | fixed | 30/50 | 10/50 | 0.000 |
| T1 | joint | 19/50 | 1/50 | 0.000 |
| T2 | safety | 20/50 | 31/50 | 0.045 |
| T2 | fixed | 33/50 | 15/50 | 0.001 |
| T2 | joint | 20/50 | 4/50 | 0.000 |
| T3 | safety | 23/50 | 17/50 | 0.307 |
| T3 | fixed | 32/50 | 16/50 | 0.003 |
| T3 | joint | 18/50 | 6/50 | 0.009 |
| T4 | safety | 22/50 | 10/50 | 0.018 |
| T4 | fixed | 31/50 | 10/50 | 0.000 |
| T4 | joint | 11/50 | 5/50 | 0.171 |

### Contamination signal (similarity to upstream fixed function)

- historical: 0/101 verbatim matches, mean similarity 0.76
- synthetic: 0/269 verbatim matches, mean similarity 0.82

T3 runs with analyzer feedback (more than one call): 3 of 100.

Generation cost: 209866 output tokens, 74.0 GPU-minutes total.
