## Overview
A small tool that can play videos on your desktop.

## Features
- Relatively small
- Multilingual support(translators needed)
- Supports almost all kinds of media files
- Supports play online media streams
- **Hardware Accelerated Decoding** support: **CUDA**, **DXVA** and **D3D11**
- Low CPU and memory usage
- Do not need any run-time libraries
- Pure green: no need to install, no write to registry and no files left in your system
- No ads, free of charge, open source, etc...

## Supported platforms
Microsoft Windows 7 ~ 10, both 32 bit and 64 bit systems are OK.

## Important notes
- Enable Windows Aero first
- To experience **Hardware Accelerated Decoding** better, make sure this tool runs with NVIDIA GPU (on Optimus)

## Compilation
- Download [Qt5](http://download.qt.io/archive/qt/) and install it.
- Download [QtAV](https://github.com/wang-bin/QtAV)'s prebuilt SDK and install it, or build yourself.
- Download [FFmpeg](https://github.com/wang-bin/avbuild)'s prebuilt binaries or build yourself.
- Open "dynamic-desktop.pro" and start compiling or call "build.bat". It's quite easy.

## Licenses
- Main train of thought comes from https://github.com/ThomasHuai/Wallpaper , some codes are also copied from https://github.com/ThomasHuai/Wallpaper/blob/master/utils.cpp and https://github.com/ThomasHuai/Wallpaper/blob/master/wallpaper.cpp , thanks to [**ThomasHuai**](https://github.com/ThomasHuai)!
- Special thanks to [wang bin](https://github.com/wang-bin) for his awesome [QtAV](https://github.com/wang-bin/QtAV)!
- Most frameless window codes are copied from https://github.com/Bringer-of-Light/Qt-Nice-Frameless-Window , thanks to [Bringer-of-Light](https://github.com/Bringer-of-Light)
- Most style sheets of Preferences Dialog are copied from https://github.com/u8sand/Baka-MPlayer , thanks to [Daniel Clarke](https://github.com/u8sand)
- [src/resources/icon.ico](/src/resources/icon.ico): downloaded from https://www.easyicon.net/
- [src/resources/minimize.svg](/src/resources/minimize.svg), [src/resources/maximize.svg](/src/resources/maximize.svg) ,[src/resources/restore.svg](/src/resources/restore.svg), [src/resources/close.svg](/src/resources/close.svg): downloaded from http://www.iconfont.cn/
- [src/resources/checked.svg](/src/resources/checked.svg), [src/resources/down-arrow.svg](/src/resources/down-arrow.svg), [src/resources/selected.svg](/src/resources/selected.svg): copied from https://github.com/u8sand/Baka-MPlayer
- Dynamic Desktop: [GPLv3](/LICENSE.md)
