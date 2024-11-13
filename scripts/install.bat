@echo off
setlocal
set "ROOT=%CD%"
set "GFILE=%CD%\tools\config\win\.gclient"
set "NO_AUTH_BOTO_CONFIG=%CD%\tools\config\win\.boto"
set "DEPOT_TOOLS_WIN_TOOLCHAIN=0"

REM Check if Git is available
where /q git || (echo Git is not installed. Please install Git. && exit /b)

REM Check if Python is available
where /q python || (echo Python is not installed. Please install Python. && exit /b)

REM Check if pip is available
where /q pip || (echo pip is not installed. Please install pip. && exit /b)

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
gclient sync