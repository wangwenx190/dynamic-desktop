@echo off
echo #pragma once>"%APPVEYOR_BUILD_FOLDER%\version_ci.h"
echo #define DD_BUILD_VERSION %APPVEYOR_BUILD_NUMBER%>>"%APPVEYOR_BUILD_FOLDER%\version_ci.h"
echo #define DD_COMMIT_ID "%APPVEYOR_REPO_COMMIT%">>"%APPVEYOR_BUILD_FOLDER%\version_ci.h"
echo #define DD_COMMIT_TIME "%APPVEYOR_REPO_COMMIT_TIMESTAMP%">>"%APPVEYOR_BUILD_FOLDER%\version_ci.h"
