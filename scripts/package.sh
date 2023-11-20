#!/bin/bash

# 函数：创建zip包
package() {
    os=$1
    libfile=$2
    extrafile=$3

    # 创建临时目录
    tmp_dir="tmp_${os}"
    mkdir -p "${tmp_dir}/lib"
    mkdir -p "${tmp_dir}/include"

    # 复制文件
    cp -R libwebrtc/include/* "${tmp_dir}/include/"
    cp LICENSE "${tmp_dir}/"
    cp "${libfile}" "${tmp_dir}/lib/"
    if [ -n "${extrafile}" ]; then
        cp "${extrafile}" "${tmp_dir}/lib/"
    fi

    # 打包
    zip -r "libwebrtc-${os}.zip" "${tmp_dir}"

    # 清理
    rm -rf "${tmp_dir}"
}

# 为Linux x64打包
package "linux-x64" "libs/linux_x64/libwebrtc.so" "libs/linux_x64/libwebrtc.so.TOC"

# 为Mac x64打包
package "mac-x64" "libs/mac_x64/libwebrtc.dylib" ""

# 为Windows x64打包
package "win-x64" "libs/win_x64/libwebrtc.dll" "libs/win_x64/libwebrtc.dll.lib"

echo "打包完成"
