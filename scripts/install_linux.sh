#!/bin/bash

export HTTPS_PROXY="http://192.168.3.157:7890"
export HTTP_PROXY="http://192.168.3.157:7890"
export NO_AUTH_BOTO_CONFIG="$PWD/linux/.boto"
# Set the current directory to the script's directory
cd "$(dirname "$0")"
ROOT="$PWD"
GFILE="$PWD/linux/.gclient"
NO_AUTH_BOTO_CONFIG="$PWD/linux/.boto"
DEPOT_TOOLS_WIN_TOOLCHAIN=0

# Check if the directory 'tools/gclient' exists
if [ ! -d "tools/gclient" ]; then
    # If not, create it
    mkdir -p "tools/gclient"
    # Clone the depot_tools into it
    git clone https://chromium.googlesource.com/chromium/tools/depot_tools.git "tools/gclient"
fi

# Check if the directory 'tools/ninja' exists
if [ ! -f "tools/linux/ninja/ninja" ]; then
    # If not, create it
    mkdir -p "tools/linux/ninja"
    # Download the ninja binary into it
    curl -L -o "tools/linux/ninja-linux.zip" "https://github.com/ninja-build/ninja/releases/download/v1.11.1/ninja-linux.zip"
    unzip "tools/linux/ninja-linux.zip" -d "tools/linux/ninja"
    rm "tools/linux/ninja-linux.zip"
fi

# Check if the directory 'tools/gn' exists
if [ ! -f "tools/linux/gn/gn" ]; then
    # If not, create it
    mkdir -p "tools/linux/gn"
    # Download the gn binary into it
    curl -L -o "tools/linux/gn-linux.zip" "https://chrome-infra-packages.appspot.com/dl/gn/gn/linux-amd64/+/latest"
    unzip "tools/linux/gn-linux.zip" -d "tools/linux/gn"
    rm "tools/linux/gn-linux.zip"
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