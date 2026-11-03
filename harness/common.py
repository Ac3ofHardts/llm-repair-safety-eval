"""Shared helpers for the CS 544 secure-repair harness.

Everything (seed prep, function extraction/splicing, building, running tests,
static analysis) lives here so run.py, eval.py and verify_seeds.py behave identically.
"""
import json
import os
import re
import shutil
import subprocess
import tempfile
from collections import Counter
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
HARNESS = ROOT / "harness"
REPOS = ROOT / "repos"
SEEDS = ROOT / "seeds"
TESTS = ROOT / "tests"
PROMPTS = ROOT / "prompts"
RESULTS = ROOT / "results"

CC = os.environ.get("CC", "gcc")
SAN_FLAGS = ["-g", "-O0", "-fsanitize=address,undefined", "-fno-sanitize-recover=all",
             "-fno-omit-frame-pointer"]
SAFETY_RULES = ["21.3", "17.2", "15.5", "14.2"]


def load_config():
    with open(HARNESS / "tasks.json") as f:
        cfg = json.load(f)
    cfg["by_id"] = {t["id"]: t for t in cfg["tasks"]}
    return cfg


def sh(cmd, cwd=None, timeout=120, env=None, input=None):
    """Run a command, never raise; return (rc, stdout, stderr)."""
    try:
        p = subprocess.run(cmd, cwd=cwd, capture_output=True, text=True,
                           timeout=timeout, env=env, input=input)
        return p.returncode, p.stdout, p.stderr
    except subprocess.TimeoutExpired as e:
        return 124, e.stdout or "", (e.stderr or "") + "\nTIMEOUT"


# --------------------------------------------------------------------------
# Source preparation
# --------------------------------------------------------------------------

def pristine_file(cfg, repo, path):
    """File content at the pinned commit (ignores any local edits in repos/)."""
    pin = cfg["repos"][repo]["pin"]
    rc, out, err = sh(["git", "-C", str(REPOS / repo), "show", f"{pin}:{path}"])
    if rc != 0:
        raise RuntimeError(f"cannot read {repo}:{path}@{pin}: {err}")
    return out


def make_workdir(cfg, task, seeded=True):
    """Temp dir with the repo files needed to build the task; seed patch applied if seeded."""
    repo = task["repo"]
    wd = Path(tempfile.mkdtemp(prefix=f"{task['id']}_"))
    for rel in cfg["repos"][repo]["copy"]:
        (wd / rel).parent.mkdir(parents=True, exist_ok=True)
        (wd / rel).write_text(pristine_file(cfg, repo, rel))
    if seeded:
        patch = SEEDS / f"{task['id']}.patch"
        rc, out, err = sh(["patch", "-p1", "-d", str(wd), "-i", str(patch)])
        if rc != 0:
            raise RuntimeError(f"seed patch {patch} failed:\n{out}\n{err}")
    return wd


# --------------------------------------------------------------------------
# C function extraction / splicing (brace matching aware of strings & comments)
# --------------------------------------------------------------------------

def _match_brace(src, open_idx):
    depth, i, n = 0, open_idx, len(src)
    while i < n:
        c = src[i]
        if src.startswith("//", i):
            i = src.find("\n", i)
            if i < 0:
                return -1
        elif src.startswith("/*", i):
            i = src.find("*/", i + 2)
            if i < 0:
                return -1
            i += 1
        elif c in "\"'":
            q = c
            i += 1
            while i < n and src[i] != q:
                i += 2 if src[i] == "\\" else 1
        elif c == "{":
            depth += 1
        elif c == "}":
            depth -= 1
            if depth == 0:
                return i
        i += 1
    return -1


def find_function(src, name):
    """Return (start, end) char offsets of the definition of `name`, or None.

    start = beginning of the line holding the return type, end = index after '}'.
    """
    pat = re.compile(r"^[A-Za-z_][\w \t\*]*?\b" + re.escape(name) + r"\s*\(", re.M)
    for m in pat.finditer(src):
        head = src[m.start():m.end()]
        if head.lstrip().startswith(("return", "if", "else", "#")):
            continue
        # find the closing paren of the parameter list, then expect '{'
        depth, i = 0, m.end() - 1
        while i < len(src):
            if src[i] == "(":
                depth += 1
            elif src[i] == ")":
                depth -= 1
                if depth == 0:
                    break
            i += 1
        j = i + 1
        while j < len(src) and src[j] in " \t\r\n":
            j += 1
        if j < len(src) and src[j] == "{":
            end = _match_brace(src, j)
            if end > 0:
                start = m.start()
                # include a 'static'/'inline' qualifier on the previous line if split
                prev = src.rfind("\n", 0, start - 1)
                prev_line = src[prev + 1:start].strip()
                if prev_line in ("static", "inline", "static inline"):
                    start = prev + 1
                return start, end + 1
    return None


def extract_function(src, name):
    span = find_function(src, name)
    return src[span[0]:span[1]] if span else None


def function_line_range(src, name):
    span = find_function(src, name)
    if not span:
        return None
    return src.count("\n", 0, span[0]) + 1, src.count("\n", 0, span[1]) + 1


def splice_function(src, name, new_func):
    span = find_function(src, name)
    if not span:
        raise ValueError(f"function {name} not found in source")
    return src[:span[0]] + new_func.strip() + src[span[1]:]


def extract_code_from_response(text, name):
    """Pull the target function out of a model response (fenced block preferred)."""
    blocks = re.findall(r"```(?:c|C|cpp|c\+\+)?\s*\n(.*?)```", text, re.S)
    candidates = blocks + [text]
    for blk in candidates:
        f = extract_function(blk, name)
        if f:
            return f
    return None


def normalize_c(code):
    code = re.sub(r"/\*.*?\*/", "", code, flags=re.S)
    code = re.sub(r"//[^\n]*", "", code)
    return re.sub(r"\s+", "", code)


# --------------------------------------------------------------------------
# Prompt context
# --------------------------------------------------------------------------

def build_context(cfg, task, wd):
    parts = []
    for item in task.get("context", []):
        f = item["file"]
        path = Path(f) if f.startswith("/") else wd / f
        if not path.exists():
            continue
        text = path.read_text(errors="replace")
        if "function" in item:
            fn = extract_function(text, item["function"])
            if fn:
                parts.append(fn)
        elif "regex" in item:
            rx = re.compile(item["regex"])
            lines = [ln for ln in text.splitlines() if rx.search(ln)]
            if lines:
                parts.append("\n".join(lines))
    return "\n\n".join(parts)


# --------------------------------------------------------------------------
# Build and run tests
# --------------------------------------------------------------------------

def include_flags(cfg, task, wd):
    flags = ["-I", str(wd)]
    for d in cfg["repos"][task["repo"]].get("include_dirs", []):
        flags += ["-I", str(wd / d)]
    return flags


def build_test(cfg, task, wd, out_bin):
    src = TESTS / f"test_{task['id']}.c"
    cmd = [CC, *SAN_FLAGS, *include_flags(cfg, task, wd), "-I", str(TESTS), str(src),
           "-o", str(out_bin), "-lm"]
    rc, out, err = sh(cmd, timeout=180)
    return rc == 0, err


def run_test(binary, mode, timeout=20):
    env = dict(os.environ,
               ASAN_OPTIONS="detect_leaks=0:abort_on_error=0:exitcode=99",
               UBSAN_OPTIONS="print_stacktrace=1:halt_on_error=1:exitcode=98")
    rc, out, err = sh([str(binary), mode], timeout=timeout, env=env)
    sanitizer = ("AddressSanitizer" in err) or ("runtime error:" in err) or rc in (98, 99)
    crashed = rc < 0 or rc == 124
    return {"rc": rc, "pass": rc == 0, "sanitizer": sanitizer, "crash": crashed,
            "stderr": err[-2000:]}


# --------------------------------------------------------------------------
# Static analysis
# --------------------------------------------------------------------------

def _in_range(line, rng):
    return rng is not None and rng[0] <= line <= rng[1]


def cppcheck_security(wd, filename, rng, incs=()):
    """cppcheck warnings (non-style) inside the function's line range."""
    tmpl = "{line}|{id}|{severity}|{message}"
    rc, out, err = sh(["cppcheck", "--enable=warning,portability", "--inconclusive",
                       "--suppress=missingIncludeSystem", "--suppress=missingInclude",
                       "--quiet", f"--template={tmpl}", *incs, str(wd / filename)],
                      timeout=300)
    found = []
    for ln in err.splitlines():
        parts = ln.split("|", 3)
        if len(parts) == 4 and parts[0].isdigit() and _in_range(int(parts[0]), rng):
            found.append({"tool": "cppcheck", "line": int(parts[0]), "rule": parts[1],
                          "message": parts[3]})
    return found


def semgrep_run(wd, filename, configs, rng):
    cmd = ["semgrep", "--json", "--quiet", "--metrics=off", "--disable-version-check"]
    for c in configs:
        cmd += ["--config", c]
    cmd.append(str(wd / filename))
    rc, out, err = sh(cmd, timeout=300)
    found = []
    try:
        data = json.loads(out or "{}")
    except json.JSONDecodeError:
        return found
    for r in data.get("results", []):
        line = r.get("start", {}).get("line", 0)
        if _in_range(line, rng):
            found.append({"tool": "semgrep", "line": line, "rule": r.get("check_id", ""),
                          "message": r.get("extra", {}).get("message", "")[:300]})
    return found


def semgrep_security(wd, filename, rng):
    # p/c = Semgrep registry C ruleset (needs network the first time it is fetched).
    # Set SEMGREP_SECURITY_CONFIG=none to use only the local generic rules.
    reg = os.environ.get("SEMGREP_SECURITY_CONFIG", "p/c")
    configs = [] if reg == "none" else [reg]
    configs.append(str(HARNESS / "rules" / "security.yml"))
    return semgrep_run(wd, filename, configs, rng)


def semgrep_loops(wd, filename, rng):
    return semgrep_run(wd, filename, [str(HARNESS / "rules" / "unbounded-loops.yml")], rng)


def _misra_addon():
    """Locate cppcheck's misra.py (override with MISRA_ADDON=/path/to/misra.py)."""
    if os.environ.get("MISRA_ADDON"):
        return os.environ["MISRA_ADDON"]
    for p in ("/usr/lib/x86_64-linux-gnu/cppcheck/addons/misra.py",
              "/usr/share/cppcheck/addons/misra.py", "/usr/lib/cppcheck/addons/misra.py",
              "/usr/local/share/cppcheck/addons/misra.py", "/opt/homebrew/share/cppcheck/addons/misra.py"):
        if Path(p).exists():
            return p
    raise RuntimeError("misra.py not found; set MISRA_ADDON")


def misra_violations(wd, filename, rng, incs=()):
    """Counter of selected MISRA C:2012 rule ids inside the function's line range.

    Runs `cppcheck --dump` then the MISRA addon directly in --cli mode. (Running it via
    `cppcheck --addon=misra` silently produced no output on some distro packages.)
    """
    target = wd / filename
    dump = Path(str(target) + ".dump")
    sh(["cppcheck", "--dump", "--quiet", "--suppress=missingIncludeSystem",
        "--suppress=missingInclude", *incs, str(target)], timeout=600)
    c = Counter()
    if not dump.exists():
        return c
    rc, out, err = sh(["python3", _misra_addon(), "--cli", str(dump)], timeout=600)
    for ln in out.splitlines():
        try:
            d = json.loads(ln)
        except json.JSONDecodeError:
            continue
        if Path(d.get("file", "")).name != Path(filename).name:
            continue
        m = re.search(r"(\d+\.\d+)$", d.get("errorId", ""))
        if m and m.group(1) in SAFETY_RULES and _in_range(int(d.get("linenr", 0)), rng):
            c[m.group(1)] += 1
    dump.unlink(missing_ok=True)
    for extra in wd.glob(filename + "*.ctu-info"):
        extra.unlink(missing_ok=True)
    return c


def security_warnings(wd, filename, rng, incs=()):
    return cppcheck_security(wd, filename, rng, incs) + semgrep_security(wd, filename, rng)


def format_warnings(warnings, rng):
    """Human-readable list for the T3 feedback prompt (lines relative to the function)."""
    lines = []
    base = rng[0] if rng else 1
    for w in warnings:
        rule = w["rule"].split(".")[-1]  # drop semgrep's path prefix
        lines.append(f"- line {w['line'] - base + 1} of the function [{w['tool']}:{rule}] "
                     f"{w['message']}")
    return "\n".join(lines)


def warning_keys(warnings):
    return Counter((w["tool"], w["rule"]) for w in warnings)
