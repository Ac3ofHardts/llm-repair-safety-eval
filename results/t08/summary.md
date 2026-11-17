# Results summary: results/t08/results.csv (400 patches)

### By technique and domain

| technique | domain | N | Extracted | Builds | Functional | CWE fixed | Safety viol. introduced | Joint success | Tool disagreement |
|---|---|---|---|---|---|---|---|---|---|
| T1 | automotive | 50 | 94% | 94% | 76% | 62% | 34% | 36% | 34% |
| T1 | avionics | 50 | 88% | 80% | 40% | 16% | 24% | 2% | 22% |
| T2 | automotive | 50 | 96% | 88% | 72% | 62% | 36% | 38% | 32% |
| T2 | avionics | 50 | 90% | 90% | 68% | 38% | 68% | 4% | 68% |
| T3 | automotive | 50 | 90% | 82% | 66% | 64% | 38% | 36% | 36% |
| T3 | avionics | 50 | 94% | 94% | 66% | 34% | 42% | 10% | 42% |
| T4 | automotive | 50 | 96% | 92% | 66% | 52% | 26% | 24% | 30% |
| T4 | avionics | 50 | 94% | 94% | 52% | 24% | 14% | 12% | 14% |

### By technique and representation

| technique | representation | N | Extracted | Builds | Functional | CWE fixed | Safety viol. introduced | Joint success | Tool disagreement |
|---|---|---|---|---|---|---|---|---|---|
| T1 | binary | 40 | 95% | 85% | 50% | 45% | 42% | 10% | 40% |
| T1 | text | 60 | 88% | 88% | 63% | 35% | 20% | 25% | 20% |
| T2 | binary | 40 | 92% | 92% | 82% | 55% | 70% | 12% | 70% |
| T2 | text | 60 | 93% | 87% | 62% | 47% | 40% | 27% | 37% |
| T3 | binary | 40 | 95% | 95% | 78% | 68% | 62% | 20% | 62% |
| T3 | text | 60 | 90% | 83% | 58% | 37% | 25% | 25% | 23% |
| T4 | binary | 40 | 95% | 95% | 72% | 50% | 28% | 25% | 28% |
| T4 | text | 60 | 95% | 92% | 50% | 30% | 15% | 13% | 18% |

### By domain and representation

| domain | representation | N | Extracted | Builds | Functional | CWE fixed | Safety viol. introduced | Joint success | Tool disagreement |
|---|---|---|---|---|---|---|---|---|---|
| automotive | binary | 40 | 100% | 100% | 100% | 100% | 65% | 35% | 65% |
| automotive | text | 160 | 92% | 86% | 62% | 50% | 26% | 33% | 25% |
| avionics | binary | 120 | 92% | 89% | 61% | 39% | 46% | 11% | 45% |
| avionics | text | 80 | 90% | 90% | 50% | 11% | 24% | 1% | 24% |

### By model and technique

| model | technique | N | Extracted | Builds | Functional | CWE fixed | Safety viol. introduced | Joint success | Tool disagreement |
|---|---|---|---|---|---|---|---|---|---|
| codestral:22b | T1 | 50 | 82% | 74% | 50% | 34% | 38% | 8% | 38% |
| codestral:22b | T2 | 50 | 86% | 78% | 56% | 50% | 52% | 10% | 50% |
| codestral:22b | T3 | 50 | 84% | 80% | 52% | 44% | 42% | 8% | 42% |
| codestral:22b | T4 | 50 | 90% | 86% | 56% | 38% | 32% | 6% | 32% |
| qwen2.5-coder:32b | T1 | 50 | 100% | 100% | 66% | 44% | 20% | 30% | 18% |
| qwen2.5-coder:32b | T2 | 50 | 100% | 100% | 84% | 50% | 52% | 32% | 50% |
| qwen2.5-coder:32b | T3 | 50 | 100% | 96% | 80% | 54% | 38% | 38% | 36% |
| qwen2.5-coder:32b | T4 | 50 | 100% | 100% | 62% | 38% | 8% | 30% | 12% |

### By task and technique

| task | technique | N | Extracted | Builds | Functional | CWE fixed | Safety viol. introduced | Joint success | Tool disagreement |
|---|---|---|---|---|---|---|---|---|---|
| A1 | T1 | 10 | 100% | 100% | 100% | 100% | 50% | 50% | 50% |
| A1 | T2 | 10 | 100% | 90% | 90% | 90% | 30% | 60% | 30% |
| A1 | T3 | 10 | 100% | 100% | 80% | 100% | 30% | 60% | 30% |
| A1 | T4 | 10 | 100% | 90% | 90% | 70% | 40% | 30% | 40% |
| A2 | T1 | 10 | 100% | 100% | 100% | 100% | 10% | 90% | 10% |
| A2 | T2 | 10 | 100% | 90% | 90% | 80% | 30% | 60% | 30% |
| A2 | T3 | 10 | 100% | 100% | 90% | 90% | 30% | 70% | 30% |
| A2 | T4 | 10 | 100% | 100% | 60% | 60% | 40% | 20% | 60% |
| A3 | T1 | 10 | 90% | 90% | 0% | 0% | 10% | 0% | 10% |
| A3 | T2 | 10 | 100% | 80% | 0% | 0% | 40% | 0% | 20% |
| A3 | T3 | 10 | 90% | 50% | 0% | 0% | 40% | 0% | 30% |
| A3 | T4 | 10 | 100% | 90% | 0% | 10% | 0% | 0% | 0% |
| A4 | T1 | 10 | 100% | 100% | 100% | 100% | 70% | 30% | 70% |
| A4 | T2 | 10 | 100% | 100% | 100% | 100% | 70% | 30% | 70% |
| A4 | T3 | 10 | 100% | 100% | 100% | 100% | 70% | 30% | 70% |
| A4 | T4 | 10 | 100% | 100% | 100% | 100% | 50% | 50% | 50% |
| A5 | T1 | 10 | 80% | 80% | 80% | 10% | 30% | 10% | 30% |
| A5 | T2 | 10 | 80% | 80% | 80% | 40% | 10% | 40% | 10% |
| A5 | T3 | 10 | 60% | 60% | 60% | 30% | 20% | 20% | 20% |
| A5 | T4 | 10 | 80% | 80% | 80% | 20% | 0% | 20% | 0% |
| V1 | T1 | 10 | 80% | 80% | 10% | 10% | 0% | 10% | 0% |
| V1 | T2 | 10 | 80% | 80% | 40% | 40% | 20% | 20% | 20% |
| V1 | T3 | 10 | 80% | 80% | 50% | 50% | 0% | 50% | 0% |
| V1 | T4 | 10 | 90% | 90% | 0% | 0% | 0% | 0% | 0% |
| V2 | T1 | 10 | 100% | 90% | 40% | 50% | 0% | 0% | 0% |
| V2 | T2 | 10 | 90% | 90% | 90% | 20% | 90% | 0% | 90% |
| V2 | T3 | 10 | 100% | 100% | 60% | 60% | 80% | 0% | 80% |
| V2 | T4 | 10 | 90% | 90% | 90% | 0% | 10% | 0% | 10% |
| V3 | T1 | 10 | 70% | 70% | 40% | 0% | 20% | 0% | 20% |
| V3 | T2 | 10 | 80% | 80% | 60% | 60% | 70% | 0% | 70% |
| V3 | T3 | 10 | 100% | 100% | 70% | 0% | 20% | 0% | 20% |
| V3 | T4 | 10 | 90% | 90% | 60% | 10% | 0% | 10% | 0% |
| V4 | T1 | 10 | 90% | 90% | 60% | 0% | 0% | 0% | 0% |
| V4 | T2 | 10 | 100% | 100% | 50% | 10% | 60% | 0% | 60% |
| V4 | T3 | 10 | 90% | 90% | 50% | 0% | 10% | 0% | 10% |
| V4 | T4 | 10 | 100% | 100% | 10% | 10% | 10% | 0% | 10% |
| V5 | T1 | 10 | 100% | 70% | 50% | 20% | 100% | 0% | 90% |
| V5 | T2 | 10 | 100% | 100% | 100% | 60% | 100% | 0% | 100% |
| V5 | T3 | 10 | 100% | 100% | 100% | 60% | 100% | 0% | 100% |
| V5 | T4 | 10 | 100% | 100% | 100% | 100% | 50% | 50% | 50% |

### By seed type (synthetic vs historical)

| seed_type | N | Extracted | Builds | Functional | CWE fixed | Safety viol. introduced | Joint success | Tool disagreement |
|---|---|---|---|---|---|---|---|---|
| historical | 120 | 85% | 79% | 44% | 15% | 22% | 8% | 19% |
| synthetic | 280 | 96% | 94% | 71% | 56% | 41% | 25% | 41% |

### Duplicate patches

- 371 extracted patches, 288 distinct within their model x task x technique cell (83 duplicates).
- codestral:22b: 143 distinct of 171
- qwen2.5-coder:32b: 145 distinct of 200

### Technique comparisons (Fisher's exact, two-sided)

| Comparison | Metric | Patches | A | B | p |
|---|---|---|---|---|---|
| T1 vs T2 | fixed | all | 39/100 | 50/100 | 0.155 |
| T1 vs T2 | fixed | distinct | 19/63 | 32/69 | 0.0735 |
| T1 vs T2 | safety | all | 29/100 | 52/100 | 0.00146 |
| T1 vs T2 | safety | distinct | 21/63 | 40/69 | 0.00538 |
| T1 vs T2 | joint | all | 19/100 | 21/100 | 0.86 |
| T1 vs T2 | joint | distinct | 7/63 | 11/69 | 0.457 |
| T1 vs T3 | fixed | all | 39/100 | 49/100 | 0.2 |
| T1 vs T3 | fixed | distinct | 19/63 | 33/70 | 0.0518 |
| T1 vs T3 | safety | all | 29/100 | 40/100 | 0.137 |
| T1 vs T3 | safety | distinct | 21/63 | 31/70 | 0.217 |
| T1 vs T3 | joint | all | 19/100 | 23/100 | 0.603 |
| T1 vs T3 | joint | distinct | 7/63 | 13/70 | 0.331 |
| T1 vs T4 | fixed | all | 39/100 | 38/100 | 1 |
| T1 vs T4 | fixed | distinct | 19/63 | 31/86 | 0.487 |
| T1 vs T4 | safety | all | 29/100 | 20/100 | 0.188 |
| T1 vs T4 | safety | distinct | 21/63 | 17/86 | 0.0862 |
| T1 vs T4 | joint | all | 19/100 | 18/100 | 1 |
| T1 vs T4 | joint | distinct | 7/63 | 14/86 | 0.477 |
| T2 vs T4 | fixed | all | 50/100 | 38/100 | 0.117 |
| T2 vs T4 | fixed | distinct | 32/69 | 31/86 | 0.249 |
| T2 vs T4 | safety | all | 52/100 | 20/100 | 3.82e-06 |
| T2 vs T4 | safety | distinct | 40/69 | 17/86 | 1.05e-06 |
| T2 vs T4 | joint | all | 21/100 | 18/100 | 0.721 |
| T2 vs T4 | joint | distinct | 11/69 | 14/86 | 1 |

### Technique comparisons, cell-level sign test (model x task cells)

| Comparison | Metric | B higher | A higher | Ties | p |
|---|---|---|---|---|---|
| T1 vs T2 | fixed | 10 | 3 | 7 | 0.0923 |
| T1 vs T2 | safety | 11 | 2 | 7 | 0.0225 |
| T1 vs T2 | joint | 4 | 1 | 15 | 0.375 |
| T1 vs T3 | fixed | 6 | 2 | 12 | 0.289 |
| T1 vs T3 | safety | 5 | 2 | 13 | 0.453 |
| T1 vs T3 | joint | 4 | 1 | 15 | 0.375 |
| T1 vs T4 | fixed | 6 | 6 | 8 | 1 |
| T1 vs T4 | safety | 5 | 7 | 8 | 0.774 |
| T1 vs T4 | joint | 5 | 4 | 11 | 1 |
| T2 vs T4 | fixed | 3 | 9 | 8 | 0.146 |
| T2 vs T4 | safety | 2 | 14 | 4 | 0.00418 |
| T2 vs T4 | joint | 4 | 5 | 11 | 1 |

### RQ2: security vs safety tool disagreement

- Security tools OK, safety checker flags new violations: 135
- Safety checker OK, security tools flag new warnings: 4
- CWE actually fixed but safety violation introduced: 85 of 176 fixes

### Introduced safety violations by rule

| Rule | Patches introducing it | Total new violations |
|---|---|---|
| 21.3 | 0 | 0 |
| 17.2 | 0 | 0 |
| 15.5 | 138 | 186 |
| 14.2 | 0 | 0 |
| loops | 4 | 7 |

### RQ3: domain and representation (Fisher's exact, two-sided)

| Comparison | Metric | Patches | A | B | p |
|---|---|---|---|---|---|
| automotive vs avionics | fixed | all | 120/200 | 56/200 | 1.52e-10 |
| automotive vs avionics | fixed | distinct | 73/140 | 42/148 | 4.24e-05 |
| automotive vs avionics | safety | all | 67/200 | 74/200 | 0.53 |
| automotive vs avionics | safety | distinct | 52/140 | 57/148 | 0.903 |
| automotive vs avionics | joint | all | 67/200 | 14/200 | 2.4e-11 |
| automotive vs avionics | joint | distinct | 35/140 | 10/148 | 1.84e-05 |
| text vs binary | fixed | all | 89/240 | 87/160 | 0.000703 |
| text vs binary | fixed | distinct | 58/182 | 57/106 | 0.000298 |
| text vs binary | safety | all | 60/240 | 81/160 | 2.36e-07 |
| text vs binary | safety | distinct | 54/182 | 55/106 | 0.000245 |
| text vs binary | joint | all | 54/240 | 27/160 | 0.204 |
| text vs binary | joint | distinct | 28/182 | 17/106 | 0.868 |
| avionics: text vs binary | fixed | all | 9/80 | 47/120 | 1.14e-05 |
| avionics: text vs binary | fixed | distinct | 9/66 | 33/82 | 0.000428 |
| avionics: text vs binary | safety | all | 19/80 | 55/120 | 0.00169 |
| avionics: text vs binary | safety | distinct | 18/66 | 39/82 | 0.017 |
| avionics: text vs binary | joint | all | 1/80 | 13/120 | 0.0094 |
| avionics: text vs binary | joint | distinct | 1/66 | 9/82 | 0.043 |
| text only: automotive vs avionics | fixed | all | 80/160 | 9/80 | 1.19e-09 |
| text only: automotive vs avionics | fixed | distinct | 49/116 | 9/66 | 5.92e-05 |
| text only: automotive vs avionics | safety | all | 41/160 | 19/80 | 0.875 |
| text only: automotive vs avionics | safety | distinct | 36/116 | 18/66 | 0.617 |
| text only: automotive vs avionics | joint | all | 53/160 | 1/80 | 5.16e-10 |
| text only: automotive vs avionics | joint | distinct | 27/116 | 1/66 | 2.54e-05 |

| Technique | Metric | Automotive | Avionics | p |
|---|---|---|---|---|
| T1 | safety | 17/50 | 12/50 | 0.378 |
| T1 | fixed | 31/50 | 8/50 | 0.000 |
| T1 | joint | 18/50 | 1/50 | 0.000 |
| T2 | safety | 18/50 | 34/50 | 0.003 |
| T2 | fixed | 31/50 | 19/50 | 0.027 |
| T2 | joint | 19/50 | 2/50 | 0.000 |
| T3 | safety | 19/50 | 21/50 | 0.838 |
| T3 | fixed | 32/50 | 17/50 | 0.005 |
| T3 | joint | 18/50 | 5/50 | 0.004 |
| T4 | safety | 13/50 | 7/50 | 0.211 |
| T4 | fixed | 26/50 | 12/50 | 0.007 |
| T4 | joint | 12/50 | 6/50 | 0.192 |

### Contamination signal (similarity to upstream fixed function)

- historical: 0/102 verbatim matches, mean similarity 0.76
- synthetic: 4/269 verbatim matches, mean similarity 0.80

T3 runs with analyzer feedback (more than one call): 10 of 100.

Generation cost: 217600 output tokens, 73.6 GPU-minutes total.
