#!/bin/sh
export PATH=`pwd`/tools/gclient:"$PATH"
export PATH=`pwd`/tools/mac/gn:"$PATH"
export PATH=`pwd`/tools/mac/ninja:"$PATH"
export PATH="$PWD/tools/gclient:$PATH"

GFILE=$(pwd)/mac/.gclient
ROOT=$(pwd)
LIBWEBRTC=$(pwd)/libwebrtc
GNFILE=$(pwd)/BUILD.gn


rsync -avu --delete $LIBWEBRTC webrtc/src/

cp $GNFILE webrtc/src/BUILD.gn
cp $GFILE webrtc/.gclient

export DEPOT_TOOLS_WIN_TOOLCHAIN=0
export ARCH=x64
cd webrtc/src
gn gen out/mac-$ARCH --args="target_os=\"mac\" target_cpu=\"$ARCH\" is_debug=false rtc_enable_google_benchmarks=false rtc_include_tests=false rtc_use_h264=true ffmpeg_branding=\"Chrome\" is_component_build=false use_rtti=true use_custom_libcxx=false rtc_enable_protobuf=false rtc_include_tests=false rtc_build_examples=false libwebrtc_desktop_capture=true mac_deployment_target=\"10.14\"" 
ninja -C out/mac-$ARCH

cd $ROOT
if [ ! -d "libs/mac_$ARCH" ]; then
    mkdir -p "libs/mac_$ARCH"
fi

cp webrtc/src/out/mac-$ARCH/libwebrtc.dylib libs/mac_$ARCH/libwebrtc.dylib