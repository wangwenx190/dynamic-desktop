@echo off
if exist ffmpeg rd /s /q ffmpeg
md ffmpeg
cd ffmpeg
appveyor DownloadFile "https://sourceforge.net/projects/avbuild/files/windows-desktop/ffmpeg-git-desktop-VS2017-lite.7z/download" -FileName qtavffmpeg.7z
7z x qtavffmpeg.7z > NUL
del /f qtavffmpeg.7z
move ffmpeg-* ffmpeg
xcopy /s /y /i ffmpeg %APPVEYOR_BUILD_FOLDER%\ffmpeg > NUL
cd ..
rd /s /q ffmpeg
