# Results summary: results/base/results.csv (210 patches)

### By technique and domain

| technique | domain | N | Extracted | Builds | Functional | CWE fixed | Safety viol. introduced | Joint success | Tool disagreement |
|---|---|---|---|---|---|---|---|---|---|
| T1 | automotive | 50 | 98% | 98% | 80% | 60% | 32% | 38% | 32% |
| T1 | avionics | 20 | 90% | 90% | 30% | 30% | 0% | 5% | 0% |
| T2 | automotive | 50 | 98% | 90% | 76% | 66% | 40% | 40% | 46% |
| T2 | avionics | 20 | 100% | 100% | 65% | 25% | 50% | 20% | 50% |
| T3 | automotive | 50 | 94% | 92% | 70% | 64% | 46% | 36% | 46% |
| T3 | avionics | 20 | 100% | 100% | 55% | 55% | 25% | 30% | 25% |

### By technique and representation

| technique | representation | N | Extracted | Builds | Functional | CWE fixed | Safety viol. introduced | Joint success | Tool disagreement |
|---|---|---|---|---|---|---|---|---|---|
| T1 | binary | 30 | 93% | 93% | 53% | 53% | 20% | 17% | 20% |
| T1 | text | 40 | 98% | 98% | 75% | 50% | 25% | 38% | 25% |
| T2 | binary | 30 | 100% | 100% | 77% | 50% | 60% | 20% | 60% |
| T2 | text | 40 | 98% | 88% | 70% | 57% | 30% | 45% | 38% |
| T3 | binary | 30 | 100% | 100% | 70% | 70% | 47% | 23% | 47% |
| T3 | text | 40 | 92% | 90% | 62% | 55% | 35% | 42% | 35% |

### By domain and representation

| domain | representation | N | Extracted | Builds | Functional | CWE fixed | Safety viol. introduced | Joint success | Tool disagreement |
|---|---|---|---|---|---|---|---|---|---|
| automotive | binary | 30 | 100% | 100% | 100% | 100% | 77% | 23% | 77% |
| automotive | text | 120 | 96% | 92% | 69% | 54% | 30% | 42% | 32% |
| avionics | binary | 60 | 97% | 97% | 50% | 37% | 25% | 18% | 25% |

### By model and technique

| model | technique | N | Extracted | Builds | Functional | CWE fixed | Safety viol. introduced | Joint success | Tool disagreement |
|---|---|---|---|---|---|---|---|---|---|
| codestral:22b | T1 | 35 | 91% | 91% | 74% | 46% | 43% | 17% | 43% |
| codestral:22b | T2 | 35 | 97% | 86% | 69% | 49% | 51% | 20% | 60% |
| codestral:22b | T3 | 35 | 91% | 89% | 60% | 43% | 37% | 14% | 37% |
| qwen2.5-coder:32b | T1 | 35 | 100% | 100% | 57% | 57% | 3% | 40% | 3% |
| qwen2.5-coder:32b | T2 | 35 | 100% | 100% | 77% | 60% | 34% | 49% | 34% |
| qwen2.5-coder:32b | T3 | 35 | 100% | 100% | 71% | 80% | 43% | 54% | 43% |

### By task and technique

| task | technique | N | Extracted | Builds | Functional | CWE fixed | Safety viol. introduced | Joint success | Tool disagreement |
|---|---|---|---|---|---|---|---|---|---|
| A1 | T1 | 10 | 100% | 100% | 100% | 100% | 50% | 50% | 50% |
| A1 | T2 | 10 | 100% | 100% | 100% | 100% | 50% | 50% | 50% |
| A1 | T3 | 10 | 100% | 100% | 90% | 100% | 50% | 50% | 50% |
| A2 | T1 | 10 | 100% | 100% | 100% | 100% | 0% | 100% | 0% |
| A2 | T2 | 10 | 100% | 100% | 100% | 100% | 0% | 100% | 0% |
| A2 | T3 | 10 | 100% | 100% | 90% | 90% | 10% | 90% | 10% |
| A3 | T1 | 10 | 90% | 90% | 0% | 0% | 0% | 0% | 0% |
| A3 | T2 | 10 | 100% | 60% | 0% | 0% | 20% | 0% | 50% |
| A3 | T3 | 10 | 100% | 90% | 0% | 0% | 40% | 0% | 40% |
| A4 | T1 | 10 | 100% | 100% | 100% | 100% | 60% | 40% | 60% |
| A4 | T2 | 10 | 100% | 100% | 100% | 100% | 80% | 20% | 80% |
| A4 | T3 | 10 | 100% | 100% | 100% | 100% | 90% | 10% | 90% |
| A5 | T1 | 10 | 100% | 100% | 100% | 0% | 50% | 0% | 50% |
| A5 | T2 | 10 | 90% | 90% | 80% | 30% | 50% | 30% | 50% |
| A5 | T3 | 10 | 70% | 70% | 70% | 30% | 40% | 30% | 40% |
| V1 | T1 | 10 | 80% | 80% | 10% | 10% | 0% | 10% | 0% |
| V1 | T2 | 10 | 100% | 100% | 40% | 40% | 10% | 40% | 10% |
| V1 | T3 | 10 | 100% | 100% | 60% | 60% | 0% | 60% | 0% |
| V2 | T1 | 10 | 100% | 100% | 50% | 50% | 0% | 0% | 0% |
| V2 | T2 | 10 | 100% | 100% | 90% | 10% | 90% | 0% | 90% |
| V2 | T3 | 10 | 100% | 100% | 50% | 50% | 50% | 0% | 50% |

### By seed type (synthetic vs historical)

| seed_type | N | Extracted | Builds | Functional | CWE fixed | Safety viol. introduced | Joint success | Tool disagreement |
|---|---|---|---|---|---|---|---|---|
| historical | 60 | 92% | 83% | 42% | 10% | 33% | 10% | 38% |
| synthetic | 150 | 99% | 99% | 79% | 74% | 36% | 41% | 36% |

### Duplicate patches

- 203 extracted patches, 98 distinct within their model x task x technique cell (105 duplicates).
- codestral:22b: 56 distinct of 98
- qwen2.5-coder:32b: 42 distinct of 105

### Technique comparisons (Fisher's exact, two-sided)

| Comparison | Metric | Patches | A | B | p |
|---|---|---|---|---|---|
| T1 vs T2 | fixed | all | 36/70 | 38/70 | 0.866 |
| T1 vs T2 | fixed | distinct | 10/29 | 15/39 | 0.803 |
| T1 vs T2 | safety | all | 16/70 | 30/70 | 0.0188 |
| T1 vs T2 | safety | distinct | 9/29 | 18/39 | 0.224 |
| T1 vs T2 | joint | all | 20/70 | 24/70 | 0.585 |
| T1 vs T2 | joint | distinct | 5/29 | 9/39 | 0.763 |
| T1 vs T3 | fixed | all | 36/70 | 43/70 | 0.306 |
| T1 vs T3 | fixed | distinct | 10/29 | 16/30 | 0.192 |
| T1 vs T3 | safety | all | 16/70 | 28/70 | 0.0446 |
| T1 vs T3 | safety | distinct | 9/29 | 13/30 | 0.422 |
| T1 vs T3 | joint | all | 20/70 | 24/70 | 0.585 |
| T1 vs T3 | joint | distinct | 5/29 | 9/30 | 0.36 |

### Technique comparisons, cell-level sign test (model x task cells)

| Comparison | Metric | B higher | A higher | Ties | p |
|---|---|---|---|---|---|
| T1 vs T2 | fixed | 3 | 1 | 10 | 0.625 |
| T1 vs T2 | safety | 7 | 1 | 6 | 0.0703 |
| T1 vs T2 | joint | 3 | 1 | 10 | 0.625 |
| T1 vs T3 | fixed | 2 | 1 | 11 | 1 |
| T1 vs T3 | safety | 5 | 1 | 8 | 0.219 |
| T1 vs T3 | joint | 2 | 2 | 10 | 1 |

### RQ2: security vs safety tool disagreement

- Security tools OK, safety checker flags new violations: 73
- Safety checker OK, security tools flag new warnings: 4
- CWE actually fixed but safety violation introduced: 44 of 117 fixes

### Introduced safety violations by rule

| Rule | Patches introducing it | Total new violations |
|---|---|---|
| 21.3 | 0 | 0 |
| 17.2 | 0 | 0 |
| 15.5 | 74 | 106 |
| 14.2 | 0 | 0 |
| loops | 0 | 0 |

### RQ3: domain and representation (Fisher's exact, two-sided)

| Comparison | Metric | Patches | A | B | p |
|---|---|---|---|---|---|
| automotive vs avionics | fixed | all | 95/150 | 22/60 | 0.000654 |
| automotive vs avionics | fixed | distinct | 31/69 | 10/29 | 0.377 |
| automotive vs avionics | safety | all | 59/150 | 15/60 | 0.0559 |
| automotive vs avionics | safety | distinct | 33/69 | 7/29 | 0.0421 |
| automotive vs avionics | joint | all | 57/150 | 11/60 | 0.0057 |
| automotive vs avionics | joint | distinct | 16/69 | 7/29 | 1 |
| text vs binary | fixed | all | 65/120 | 52/90 | 0.674 |
| text vs binary | fixed | distinct | 18/56 | 23/42 | 0.038 |
| text vs binary | safety | all | 36/120 | 38/90 | 0.0801 |
| text vs binary | safety | distinct | 23/56 | 17/42 | 1 |
| text vs binary | joint | all | 50/120 | 18/90 | 0.00101 |
| text vs binary | joint | distinct | 13/56 | 10/42 | 1 |

| Technique | Metric | Automotive | Avionics | p |
|---|---|---|---|---|
| T1 | safety | 16/50 | 0/20 | 0.003 |
| T1 | fixed | 30/50 | 6/20 | 0.034 |
| T1 | joint | 19/50 | 1/20 | 0.007 |
| T2 | safety | 20/50 | 10/20 | 0.594 |
| T2 | fixed | 33/50 | 5/20 | 0.003 |
| T2 | joint | 20/50 | 4/20 | 0.164 |
| T3 | safety | 23/50 | 5/20 | 0.176 |
| T3 | fixed | 32/50 | 11/20 | 0.589 |
| T3 | joint | 18/50 | 6/20 | 0.782 |

### Contamination signal (similarity to upstream fixed function)

- historical: 0/55 verbatim matches, mean similarity 0.77
- synthetic: 0/148 verbatim matches, mean similarity 0.81

T3 runs with analyzer feedback (more than one call): 3 of 70.

Generation cost: 103196 output tokens, 35.2 GPU-minutes total.
