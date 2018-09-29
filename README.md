## Overview
A small tool that can play videos on your desktop.

## Features
- Relatively small
- Multilingual support(translators needed)
- Supports almost all kinds of media files
- Supports play online media streams
- **Hardware Accelerated Decoding** support: **CUDA**, **D3D11** and **DXVA**
- Low CPU and memory usage
- High performance
- High video quality
- Do not need any run-time libraries
- Pure green: no need to install, no write to registry and no files left in your system
- No ads, free of charge, open source, etc...

## Supported platforms
Microsoft Windows 7 ~ 10, both 32 bit and 64 bit systems.

## Important notes
- Enable Windows Aero first
- Make sure you have installed XAudio2(you can find it's installer in DirectX's installer files)
- To experience **Hardware Accelerated Decoding** better, make sure this tool runs with your high performance GPU(on Optimus)
- The CPU and memory usage will increase enormously if you are using software decoding
- It would be nice if you update your graphics card driver to the latest version

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
- [src/images/bee.ico](/src/images/bee.ico): downloaded from https://www.easyicon.net/
- [src/images/minimize.ico](/src/images/minimize.ico), [src/images/maximize.ico](/src/images/maximize.ico), [src/images/restore.ico](/src/images/restore.ico), [src/images/close.ico](/src/images/close.ico): original svg files downloaded from http://www.iconfont.cn/ , and I converted them to ico files manually
- [src/images/checked.ico](/src/images/checked.ico), [src/images/down-arrow.ico](/src/images/down-arrow.ico), [src/images/selected.ico](/src/images/selected.ico): original svg files copied from https://github.com/u8sand/Baka-MPlayer , and I converted them to ico files manually
- Dynamic Desktop: [GPLv3](/LICENSE.md)
