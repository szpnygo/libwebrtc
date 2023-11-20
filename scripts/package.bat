@echo off
setlocal

set "ROOT=%CD%"

REM 为Linux x64打包
set os=linux-x64
set libpath=libs\linux_x64\libwebrtc.so
set extrapath=libs\linux_x64\libwebrtc.so.TOC

set tmp_dir=%ROOT%\tmp_%os%
if not exist "%tmp_dir%" md "%tmp_dir%"
md "%tmp_dir%\lib"
md "%tmp_dir%\include"

xcopy /E /I "%ROOT%\libwebrtc\include\" "%tmp_dir%\include\"
copy "%ROOT%\LICENSE" "%tmp_dir%\"
copy "%ROOT%\%libpath%" "%tmp_dir%\lib\"
if not "%extrapath%"=="" copy "%ROOT%\%extrapath%" "%tmp_dir%\lib\"

powershell Compress-Archive -Path "%tmp_dir%\*" -DestinationPath "%ROOT%\libwebrtc-%os%.zip"
rd /s /q "%tmp_dir%"

REM 为Mac x64打包
set os=mac-x64
set libpath=libs\mac_x64\libwebrtc.dylib
set extrapath=

set tmp_dir=%ROOT%\tmp_%os%
if not exist "%tmp_dir%" md "%tmp_dir%"
md "%tmp_dir%\lib"
md "%tmp_dir%\include"

xcopy /E /I "%ROOT%\libwebrtc\include\" "%tmp_dir%\include\"
copy "%ROOT%\LICENSE" "%tmp_dir%\"
copy "%ROOT%\%libpath%" "%tmp_dir%\lib\"

powershell Compress-Archive -Path "%tmp_dir%\*" -DestinationPath "%ROOT%\libwebrtc-%os%.zip"
rd /s /q "%tmp_dir%"

REM 为Windows x64打包
set os=win-x64
set libpath=libs\win_x64\libwebrtc.dll
set extrapath=libs\win_x64\libwebrtc.dll.lib

set tmp_dir=%ROOT%\tmp_%os%
if not exist "%tmp_dir%" md "%tmp_dir%"
md "%tmp_dir%\lib"
md "%tmp_dir%\include"

xcopy /E /I "%ROOT%\libwebrtc\include\" "%tmp_dir%\include\"
copy "%ROOT%\LICENSE" "%tmp_dir%\"
copy "%ROOT%\%libpath%" "%tmp_dir%\lib\"
if not "%extrapath%"=="" copy "%ROOT%\%extrapath%" "%tmp_dir%\lib\"

powershell Compress-Archive -Path "%tmp_dir%\*" -DestinationPath "%ROOT%\libwebrtc-%os%.zip"
rd /s /q "%tmp_dir%"

echo 打包完成
endlocal
