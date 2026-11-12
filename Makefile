# One entry point for the whole study. Run `make help` for the list.
#
#   make setup verify            # once, on a new machine
#   make run TAG=base            # GPU: generate patches (resumes if interrupted)
#   make eval TAG=base           # score patches, write results/<tag>/summary.md
#   make tables paper            # regenerate paper tables and build the PDF
#   make reproduce               # no GPU: re-score committed generations -> tables -> paper

PY      ?= python3
TAG     ?= base
JOBS    ?= 4
MODELS  ?= qwen2.5-coder:32b codestral:22b
HOST    ?= http://localhost:11434

.PHONY: help setup verify smoke run run-base run-t4 run-t08 eval summarize coding tables paper reproduce clean

help:
	@echo "setup       clone/pin upstream repos, build dump1090 objects, install semgrep"
	@echo "verify      check all seeds (original passes, seeded fails its trigger)"
	@echo "smoke       1-repetition sanity run on V3 and V5 (needs Ollama)"
	@echo "run         generate patches: TAG=base|t08 [ARGS='--tasks A1 --techniques T4']"
	@echo "run-base    canonical run: all tasks, T1-T4, temperature 0.2, 5 reps"
	@echo "run-t08     robustness run: temperature 0.8, T1-T4"
	@echo "eval        score results/TAG/generations.jsonl and summarize"
	@echo "coding      export the qualitative coding sheet for TAG"
	@echo "tables      regenerate paper/tables/ from results/TAG (default base)"
	@echo "paper       build paper/main.pdf with latexmk (or upload paper/ to Overleaf)"
	@echo "reproduce   verify + eval + tables + paper from committed data (no GPU)"

setup:
	./setup.sh
	$(PY) -m pip install -r requirements.txt

verify:
	$(PY) harness/verify_seeds.py

smoke:
	$(PY) harness/run.py --host $(HOST) --models $(firstword $(MODELS)) --tasks V3 V5 --techniques T1 T4 --reps 1 --tag smoke
	$(PY) harness/eval.py --tag smoke --jobs $(JOBS)

run:
	$(PY) harness/run.py --host $(HOST) --models $(MODELS) --tag $(TAG) $(ARGS)

run-base:
	$(PY) harness/run.py --host $(HOST) --models $(MODELS) --tag base --techniques T1 T2 T3 T4

run-t08:
	$(PY) harness/run.py --host $(HOST) --models $(MODELS) --tag t08 --temperature 0.8 --techniques T1 T2 T3 T4

eval:
	$(PY) harness/eval.py --tag $(TAG) --jobs $(JOBS)
	$(PY) analysis/summarize.py --tag $(TAG)

summarize:
	$(PY) analysis/summarize.py --tag $(TAG)

coding:
	$(PY) analysis/code_export.py --tag $(TAG)

tables:
	$(PY) analysis/make_tables.py --tag $(TAG)

paper:
	@command -v latexmk >/dev/null || { echo "latexmk not found: upload paper/ to Overleaf instead"; exit 1; }
	cd paper && latexmk -pdf -interaction=nonstopmode main.tex

reproduce: verify
	$(PY) harness/eval.py --tag base --jobs $(JOBS)
	$(PY) analysis/summarize.py --tag base
	@if [ -s results/t08/generations.jsonl ]; then \
	  $(PY) harness/eval.py --tag t08 --jobs $(JOBS) && $(PY) analysis/summarize.py --tag t08; fi
	$(PY) analysis/make_tables.py --tag base
	-$(MAKE) paper

clean:
	cd paper && latexmk -C main.tex 2>/dev/null || true
	rm -rf results/smoke harness/__pycache__ analysis/__pycache__
