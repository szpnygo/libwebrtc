@echo off
setlocal
cd /d %~dp0
set "ROOT=%CD%"
set "GFILE=%CD%\.gclient"
set "NO_AUTH_BOTO_CONFIG=%CD%\.boto"
set "DEPOT_TOOLS_WIN_TOOLCHAIN=0"

REM Check if the directory 'tools\gclient' exists
if not exist "tools\gclient" (
    REM If not, create it
    mkdir "tools\gclient"
    REM Clone the depot_tools into it
    git clone https://chromium.googlesource.com/chromium/tools/depot_tools.git tools\gclient
)

REM Check if the directory 'tools\ninja' exists
if not exist "tools\ninja\ninja.exe" (
    REM If not, create it
    mkdir "tools\ninja"
    REM Download the gn.exe into it
    curl -L -o tools\ninja-win.zip https://github.com/ninja-build/ninja/releases/download/v1.11.1/ninja-win.zip
    tar -xf .\tools\ninja-win.zip -C .\tools\ninja
    del .\tools\ninja-win.zip
)

REM Check if the directory 'tools\gn' exists
if not exist "tools\gn\gn.exe" (
    REM If not, create it
    mkdir "tools\gn"
    REM Download the gn.exe into it
    curl -L -o tools\gn-win.zip https://chrome-infra-packages.appspot.com/dl/gn/gn/windows-amd64/+/latest
    tar -xf .\tools\gn-win.zip -C .\tools\gn
    del .\tools\gn-win.zip
)

REM Add the depot_tools directory to the PATH environment variable
set "PATH=%CD%\tools\gclient;%PATH%"
REM Check if gclient is available
where /q gclient || (echo I require gclient but it's not installed. Aborting. && exit /b)

if not exist "webrtc" (
    REM If not, create it
    mkdir "webrtc"
)

copy /Y "%GFILE%" "webrtc\.gclient"
cd webrtc
gclient sync --no-history