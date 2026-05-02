#!/bin/bash
set -euo pipefail

# ============================================================
#  CMSIS downloader (specific version)
#  - Clones ARM-software/CMSIS_5
#  - Checks out the specified tag (e.g., 5.9.0)
#  - Places it under external/cmsis/
# ============================================================

VERSION="5.9.0"
TARGET_DIR="external/cmsis/"

if [ -d "${TARGET_DIR}" ]; then
    echo "CMSIS already exists at ${TARGET_DIR}"
    exit 0
fi

mkdir -p external

echo "Cloning CMSIS..."
git clone --depth=1 --branch ${VERSION} https://github.com/ARM-software/CMSIS_5.git "${TARGET_DIR}"

echo "Done. CMSIS ${VERSION} is available under ${TARGET_DIR}"
