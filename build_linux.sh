#!/bin/sh
export PATH=`pwd`/tools/gclient:"$PATH"
export PATH=`pwd`/tools/linux/gn:"$PATH"
export PATH=`pwd`/tools/linux/ninja:"$PATH"
export PATH="$PWD/tools/gclient:$PATH"

GFILE=$(pwd)/linux/.gclient
ROOT=$(pwd)
LIBWEBRTC=$(pwd)/libwebrtc
GNFILE=$(pwd)/BUILD.gn


rsync -avu --delete $LIBWEBRTC webrtc/src/

cp $GNFILE webrtc/src/BUILD.gn
cp $GFILE webrtc/.gclient

export DEPOT_TOOLS_WIN_TOOLCHAIN=0
export ARCH=x64
cd webrtc/src
gn gen out/linux-$ARCH --args="target_os=\"linux\" target_cpu=\"$ARCH\" is_debug=false rtc_enable_google_benchmarks=false rtc_include_tests=false rtc_use_h264=true ffmpeg_branding=\"Chrome\" is_component_build=false use_rtti=true use_custom_libcxx=false rtc_enable_protobuf=false rtc_include_tests=false rtc_build_examples=false libwebrtc_desktop_capture=true use_cxx17=true" 
ninja -C out/linux-$ARCH

cd $ROOT
if [ ! -d "libs/linux_$ARCH" ]; then
    mkdir -p "libs/linux_$ARCH"
fi

cp webrtc/src/out/linux-$ARCH/libwebrtc.so libs/linux_$ARCH/libwebrtc.so
cp webrtc/src/out/linux-$ARCH/libwebrtc.so.TOC libs/linux_$ARCH/libwebrtc.so.TOC