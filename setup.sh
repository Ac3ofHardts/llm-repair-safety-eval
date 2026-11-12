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
echo "Building dump1090 objects for the linked test drivers (V3-V5) ..."
D1090_MAKE=(make -C repos/dump1090 -j4 RTLSDR=no BLADERF=no HACKRF=no LIMESDR=no SOAPYSDR=no)
if ! "${D1090_MAKE[@]}" dump1090 >/dev/null 2>repos/dump1090-build.log; then
  # dump1090 builds with -Werror; newer compilers (e.g. GCC 15) can add warnings that break it.
  echo "  build with -Werror failed; retrying without it (see repos/dump1090-build.log)"
  "${D1090_MAKE[@]}" clean >/dev/null 2>&1 || true
  "${D1090_MAKE[@]}" DUMP1090_CFLAGS="-std=c11 -fno-common -Wall -W" dump1090 >/dev/null 2>>repos/dump1090-build.log \
    || { echo "dump1090 build failed (missing libncurses-dev?); see repos/dump1090-build.log"; exit 1; }
fi
echo "ok"

echo
echo "Tool versions (record these in the paper):"
gcc --version | head -1
cppcheck --version
semgrep --version
ollama --version 2>/dev/null || echo "ollama: not on PATH (ok if running elsewhere)"
python3 --version
