@echo off
if exist "%~dp0..\build\bin64\launcher.exe" (
    echo DD_MAJOR_VERSION = 1>"%APPVEYOR_BUILD_FOLDER%\.qmake.conf"
    echo DD_MINOR_VERSION = 0>>"%APPVEYOR_BUILD_FOLDER%\.qmake.conf"
    echo DD_PATCH_VERSION = 0>>"%APPVEYOR_BUILD_FOLDER%\.qmake.conf"
    echo DD_BUILD_VERSION = %APPVEYOR_BUILD_NUMBER%>>"%APPVEYOR_BUILD_FOLDER%\.qmake.conf"
    echo DD_COMMIT_ID = %APPVEYOR_REPO_COMMIT%>>"%APPVEYOR_BUILD_FOLDER%\.qmake.conf"
    echo DD_COMMIT_TIME  = %APPVEYOR_REPO_COMMIT_TIMESTAMP%>>"%APPVEYOR_BUILD_FOLDER%\.qmake.conf"
    echo FFMPEG_VERSION_STR = Unknown>>"%APPVEYOR_BUILD_FOLDER%\.qmake.conf"
)
