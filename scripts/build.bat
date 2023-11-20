@echo on
setlocal
cd /d %~dp0
set "PATH=%CD%\tools\gclient;%PATH%"
set "PATH=%CD%\tools\gn;%PATH%"
set "PATH=%CD%\tools\ninja;%PATH%"
set "GFILE=%CD%\.gclient"
set "ROOT=%CD%"
set "LIBWEBRTC=%CD%\libwebrtc"
set "GNFILE=%CD%\BUILD.gn"

REM Delete the destination folder if it exists, then copy the new files
if exist "webrtc\src\libwebrtc" (
    rmdir /s /q "webrtc\src\libwebrtc"
)
xcopy /E /I /Y "%LIBWEBRTC%" "webrtc\src\libwebrtc\"

copy /Y "%GNFILE%" "webrtc\src\BUILD.gn"
copy /Y "%GFILE%" "webrtc\.gclient"

set DEPOT_TOOLS_WIN_TOOLCHAIN=0
set ARCH=x64
set GYP_MSVS_VERSION=2022
set GYP_GENERATORS=ninja,msvs-ninja
set GYP_MSVS_OVERRIDE_PATH=C:\Program Files\Microsoft Visual Studio\2022\Community
cd webrtc\src
gn gen "out/win-%ARCH%" --args="target_os=\"win\" target_cpu=\"%ARCH%\" is_debug=false is_clang=true rtc_include_tests=false rtc_use_h264=true ffmpeg_branding=\"Chrome\" is_component_build=false use_rtti=true rtc_enable_protobuf=false rtc_include_tests=false rtc_build_examples=false libwebrtc_desktop_capture=true" --ide=vs2022
ninja -C "out/win-%ARCH%"

cd "%ROOT%"
if not exist "libs\win_%ARCH%" mkdir "libs\win_%ARCH%"
copy /Y "webrtc\src\out\win-%ARCH%\libwebrtc.dll" "libs\win_%ARCH%\libwebrtc.dll"
copy /Y "webrtc\src\out\win-%ARCH%\libwebrtc.dll.lib" "libs\win_%ARCH%\libwebrtc.dll.lib"