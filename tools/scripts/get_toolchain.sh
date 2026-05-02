#!/bin/bash
set -euo pipefail

# ============================================================
#  ARM GNU Toolchain downloader
#  - Downloads and extracts the specified version
#  - Places it under tools/toolchain/
# ============================================================

VERSION="13.2.Rel1"
BASEFILE="arm-gnu-toolchain-${VERSION}-x86_64-arm-none-eabi"
URL="https://developer.arm.com/-/media/Files/downloads/gnu/${VERSION}/binrel/${BASEFILE}.tar.xz"

TARGET_DIR="external/toolchain/"
ARCHIVE="${TARGET_DIR}${BASEFILE}.tar.xz"

mkdir -p "${TARGET_DIR}"

echo "Downloading ARM GNU Toolchain: ${BASEFILE}"
curl -L -o "${ARCHIVE}" "${URL}"

echo "Extracting..."
tar -xJf "${ARCHIVE}" -C "${TARGET_DIR}"

echo "Done. Toolchain is available under ${TARGET_DIR}${BASEFILE}"
