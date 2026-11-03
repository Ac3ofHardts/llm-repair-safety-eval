#!/usr/bin/env bash
# Clone both subject repos into repos/ and check out the pinned commits.
set -euo pipefail
cd "$(dirname "$0")"
mkdir -p repos results
clone_pin () {
  local name=$1 url=$2 pin=$3
  [ -d "repos/$name/.git" ] || git clone -q "$url" "repos/$name"
  git -C "repos/$name" fetch -q --all --tags
  git -C "repos/$name" checkout -q "$pin"
  echo "$name pinned at $(git -C repos/$name rev-parse HEAD) ($(git -C repos/$name log -1 --format=%ci))"
}
clone_pin can-utils https://github.com/linux-can/can-utils.git 834ceef1f1eabec67625389172eebf1050d2f42b
clone_pin dump1090  https://github.com/flightaware/dump1090.git 0339a57b89cd6e61856cbb13ae342c31ae7be5ac
echo
echo "Tool versions (record these in the paper):"
gcc --version | head -1
cppcheck --version
semgrep --version
ollama --version 2>/dev/null || echo "ollama: not on PATH (ok if running elsewhere)"
python3 --version
