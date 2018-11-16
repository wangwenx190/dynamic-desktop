@echo off
if exist qtavffmpeg rd /s /q qtavffmpeg
md qtavffmpeg
cd qtavffmpeg
appveyor DownloadFile "https://sourceforge.net/projects/avbuild/files/windows-desktop/ffmpeg-git-desktop-VS2017-lite.7z/download" -FileName avbuild.7z
7z x avbuild.7z > NUL
del /f avbuild.7z
move ffmpeg-* ffmpeg
xcopy /s /y /i ffmpeg %APPVEYOR_BUILD_FOLDER%\ffmpeg > NUL
cd ..
rd /s /q qtavffmpeg
