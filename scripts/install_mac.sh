#!/bin/bash

export NO_AUTH_BOTO_CONFIG="$PWD/mac/.boto"
ROOT="$PWD"
GFILE="$PWD/tools/config/mac/.gclient"
NO_AUTH_BOTO_CONFIG="$PWD/tools/config/mac/.boto"
DEPOT_TOOLS_WIN_TOOLCHAIN=0

# Check if the directory 'tools/gclient' exists
if [ ! -d "tools/gclient" ]; then
    # If not, create it
    mkdir -p "tools/gclient"
    # Clone the depot_tools into it
    git clone https://chromium.googlesource.com/chromium/tools/depot_tools.git "tools/gclient"
fi

# Check if the directory 'tools/ninja' exists
if [ ! -f "tools/mac/ninja/ninja" ]; then
    # If not, create it
    mkdir -p "tools/mac/ninja"
    # Download the ninja binary into it
    curl -L -o "tools/mac/ninja-mac.zip" "https://github.com/ninja-build/ninja/releases/download/v1.11.1/ninja-mac.zip"
    unzip "tools/mac/ninja-mac.zip" -d "tools/mac/ninja"
    rm "tools/mac/ninja-mac.zip"
fi

# Check if the directory 'tools/gn' exists
if [ ! -f "tools/mac/gn/gn" ]; then
    # If not, create it
    mkdir -p "tools/mac/gn"
    # Download the gn binary into it
    curl -L -o "tools/mac/gn-mac.zip" "https://chrome-infra-packages.appspot.com/dl/gn/gn/mac-amd64/+/latest"
    unzip "tools/mac/gn-mac.zip" -d "tools/mac/gn"
    rm "tools/mac/gn-mac.zip"
fi

# Add the depot_tools directory to the PATH environment variable
export PATH="$PWD/tools/gclient:$PATH"
# Check if gclient is available
command -v gclient >/dev/null || { echo "I require gclient but it's not installed. Aborting."; exit 1; }

# Check if the 'webrtc' directory exists
if [ ! -d "webrtc" ]; then
    # If not, create it
    mkdir "webrtc"
fi

# Copy the .gclient file
cp -f "$GFILE" "webrtc/.gclient"
cd webrtc
gclient sync --no-history