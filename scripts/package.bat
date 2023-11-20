@echo off
setlocal

REM 函数：打包库
:package
set os=%1
set libfile=%2
set extrafile=%3

REM 创建临时目录
set tmp_dir=tmp_%os%
md %tmp_dir%\lib
md %tmp_dir%\include

REM 复制文件
xcopy /E /I libwebrtc\include\* %tmp_dir%\include\
copy LICENSE %tmp_dir%\
copy %libfile% %tmp_dir%\lib\
if not "%extrafile%" == "" copy %extrafile% %tmp_dir%\lib\

REM 打包
powershell Compress-Archive -Path %tmp_dir%\* -DestinationPath libwebrtc-%os%.zip

REM 清理
rd /s /q %tmp_dir%

REM 跳转回调用点
goto :eof

REM 为Linux x64打包
call :package linux-x64 libs\linux_x64\libwebrtc.so libs\linux_x64\libwebrtc.so.TOC

REM 为Mac x64打包
call :package mac-x64 libs\mac_x64\libwebrtc.dylib

REM 为Windows x64打包
call :package win-x64 libs\win_x64\libwebrtc.dll libs\win_x64\libwebrtc.dll.lib

echo 打包完成
endlocal
