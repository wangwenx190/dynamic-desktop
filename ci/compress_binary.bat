@echo off
setlocal
cls
set _upx_command=--ultra-brute --overlay=strip --force
set _build_dir=%~dp0..\build
set _bin_dir=%_build_dir%\bin
if exist "%_bin_dir%64" (
    cd /d "%_bin_dir%64"
) else (
    cd /d "%_bin_dir%"
)
upx %_upx_command% "DD*.exe"
upx %_upx_command% "DD*.dll"
upx %_upx_command% "lib*.dll"
upx %_upx_command% "open*.dll"
upx %_upx_command% "Qt*.dll"
upx %_upx_command% "av*.dll"
upx %_upx_command% "postproc.dll"
upx %_upx_command% "sw*.dll"
upx %_upx_command% "soft_oal.dll"
endlocal
pause
exit /b
