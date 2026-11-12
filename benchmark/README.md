# Benchmark: 10 seeded weaknesses in transportation C code

Each task is **one real function** from an open-source transportation code base with **one
seeded weakness**. Every task ships with a trigger test that demonstrates the weakness and a
functional test of normal behavior, and is verified before use.

| ID | Repo | Function | Seeded weakness | CWE | Seed type | Repr. | Trigger caught by |
|---|---|---|---|---|---|---|---|
| A1 | can-utils | `hexstring2data` | upper length bound removed | 787 | synthetic | text | ASan heap overflow |
| A2 | can-utils | `hexstring2data` | odd-length check removed | 20 | synthetic | text | assertion |
| A3 | can-utils | `parse_canframe` | delimiter reads without length checks | 125 | historical (`cbbad5e`, 2026) | text | ASan heap over-read |
| A4 | can-utils | `can_fd_dlc2len` | 4-bit index mask removed | 125 | synthetic | binary | ASan global over-read |
| A5 | can-utils | `snprintf_error_data` | `snprintf` truncation unchecked | 787 | historical (`ef853f5`, 2022) | text | ASan heap overflow |
| V1 | dump1090 | `cprNFunction` | lower clamp on zone count removed | 369 | synthetic | binary | UBSan divide by zero |
| V2 | dump1090 | `decodeCPRairborne` | latitude range check removed | 20 | synthetic | binary | assertion |
| V3 | dump1090 | `decodeHexMessage` | AVR header read without length checks | 125 | historical (`65cd5fe`, 2021) | text | ASan heap over-read |
| V4 | dump1090 | `decodeHexMessage` | message-length allowlist removed | 787 | synthetic | text | ASan stack overflow |
| V5 | dump1090 | `modeCToModeA` | table-index range check removed | 125 | synthetic | binary | ASan global over-read |

**Pinned commits:** can-utils `834ceef` (master, 2026-10-14) and dump1090 `0339a57` (v11.1, 2026-07-01).
`setup.sh` fetches both. They are not redistributed here.

## Files

| Path | Contents |
|---|---|
| `tasks.json` | Task definitions: file, function, domain, representation, CWE, seed type, prompt context, build and link settings |
| `seeds/<ID>.patch` | The seed, a unified diff against the pinned commit (`patch -p1`) |
| `tests/test_<ID>.c` | Driver with two modes: `functional` (normal behavior) and `trigger` (exercises the weakness) |
| `tests/*.h` | Shared helpers: exact-size heap copies so ASan catches over-reads, plus dump1090 setup |

## Seed types

- **Synthetic** seeds remove or weaken one existing defensive check.
- **Historical** seeds restore the vulnerable logic that a real upstream commit fixed. Where the
  literal reverse patch no longer applies (A5, V3), the pre-fix logic is rebuilt on current code.

## Verification

```bash
make verify        # or: python3 harness/verify_seeds.py
```
A seed counts as verified when the **original** function passes both test modes and the
**seeded** function fails its trigger test (through a sanitizer report or an assertion). Results are written to
`results/base/seed_verification.csv`. All 10 tasks pass.

The drivers include the target `.c` file directly so they can reach `static` functions. V3–V5
also link against the rest of dump1090 (every object except `dump1090.o` and the file under
test); `tests/test_d1090_common.h` supplies the globals that `dump1090.o` would normally define.

## Adding a task

1. Make the change in `repos/<repo>` at the pinned commit, then `git diff > benchmark/seeds/<ID>.patch`.
2. Write `tests/test_<ID>.c` with `run_functional()` and `run_trigger()`.
3. Add an entry to `tasks.json`, including `representation` and the prompt `context`.
4. `make verify` must report `OK` for the new task.
