@echo off
if exist "%~dp0..\bin64\dd64.exe" (
echo DD_VERSION         = %APPVEYOR_BUILD_VERSION%>"%APPVEYOR_BUILD_FOLDER%\version_ci.pri"
echo DD_COMMIT_ID       = %APPVEYOR_REPO_COMMIT%>>"%APPVEYOR_BUILD_FOLDER%\version_ci.pri"
echo DD_COMMIT_TIME     = %APPVEYOR_REPO_COMMIT_TIMESTAMP%>>"%APPVEYOR_BUILD_FOLDER%\version_ci.pri"
echo FFMPEG_VERSION_STR = Unknown>>"%APPVEYOR_BUILD_FOLDER%\version_ci.pri"
)
