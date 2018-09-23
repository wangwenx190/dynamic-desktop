## Overview
A small tool that can play videos on your desktop.

## Features
- Small, only about 20MB
- Multilingual support(translators needed)
- Supports almost all kinds of media files
- Supports play online media streams
- **Hardware Accelerated Decoding** support: **CUDA** and **DXVA/D3D11**
- Low CPU and memory usage
- Do not need any run-time library
- Pure green: do not need install, do not write registry and no files left in your system
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
- Open "dynamicdesktop.pro" and compile. It's quite easy.

## License
- Main train of thought comes from https://github.com/ThomasHuai/Wallpaper , some codes are also copied from https://github.com/ThomasHuai/Wallpaper/blob/master/utils.cpp and https://github.com/ThomasHuai/Wallpaper/blob/master/wallpaper.cpp , thanks to [**ThomasHuai**](https://github.com/ThomasHuai)!
- Special thanks to [wang bin](https://github.com/wang-bin) for his awesome [QtAV](https://github.com/wang-bin/QtAV)!
- [icon.ico](/icon.ico): downloaded from https://www.easyicon.net/
- [GPLv3](/LICENSE.md)
