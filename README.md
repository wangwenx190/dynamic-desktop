[![Build status](https://ci.appveyor.com/api/projects/status/7n23thxiormp6oar/branch/develop?svg=true)](https://ci.appveyor.com/project/wangwenx190/dynamic-desktop/branch/develop)

## Overview
A small tool that can play videos on your desktop. Only for Microsoft Windows 7 ~ 10(with GUI desktop). It may have some limitations because it's just a media player, putting it's window under your desktop icons. Maybe Wallpaper Engine's principle is totally different with this tool.

I created this tool only to play my favorite animation clips on my desktop, so I will never make it just like Wallpaper Engine. This tool will stay simple and will only play videos. It doesn't have as many functions as Wallpaper Engine has, but that's indeed what I want. Although this tool has the ability to show executables and web pages on your desktop, I don't want to add this kind of functions. However, if you really need it, you can fork this repository and do it yourself.

**Suggestions are welcome. And I'll be very happy if anyone reports any issue to me.**

## Features
- Relatively small
- Multilingual support(**translators needed**)
- Supports almost all kinds of media files
- Supports play online media streams
- **Hardware Accelerated Decoding** support: **CUDA**, **D3D11** and **DXVA**
- Low CPU and memory usage
- High performance
- High video quality
- HiDPI support
- Multiscreen support
- Abundant options
- Pure green: no need to install, no write to registry and no files left in your system
- No ads, free of charge, open source, etc...

## Supported platforms
Microsoft Windows 7 ~ 10(with GUI desktop), both 32 bit and 64 bit systems.

## Important notes
- Windows Vista and XP are not supported.
- This tool won't run if Windows Aero is disabled.
- Win7 users only: make sure you have installed XAudio2(Win 8/8.1/10 have installed it already by default).
- To experience *Hardware Accelerated Decoding* better, make sure this tool runs with your high performance GPU(on Optimus).
- The CPU and memory usage will increase enormously if you are using software decoding and that's normal, all media players will, don't worry.
- It would be very nice if you update your graphics card driver to the latest version.
- It's not recommended to run this tool in virtual machines, because the graphics card drivers of virtual machines are usually out-dated and even not completed.
- If there is no picture but sound, please switch the hardware decoding algorithm or just use software decoding. Because hardware decoding has many limitations while software decoding has not. In one word, **IF YOU ENCOUNTER WITH ANY STRANGE PHENOMENA, TURN OFF HARDWARE DECODING**.
- It's not recommended to change rendering engine again and again while it is playing videos, otherwise this tool is very likely to crash and I'm not able to help you in this kind of situation.
- Currently, hardware decoding only supports traditional video formats such as AVC/H.264, please do not attempt to play HEVC videos.

## Compilation
- Download [Qt5](http://download.qt.io/archive/qt/) and install it.
- Download [QtAV](https://github.com/wang-bin/QtAV)'s prebuilt SDK and install it, or build yourself.
- Download [FFmpeg](https://github.com/wang-bin/avbuild)'s prebuilt binaries or build yourself.
- Open "dynamic-desktop.pro" and start compiling or call "build.bat". But remember to call "vcvarsall.bat" and add Qt's directories to your path variables before calling "build.bat".

**IMPORTANT NOTE**

You can also use Intel C++ Compiler(ICC), Clang or MinGW to compile it, just remember to set up environment variables. Cross compile on other platforms may also work, but it will never run on platforms other than MS Windows.

## Contributing
See [CONTRIBUTING.md](/CONTRIBUTING.md) for more information.

## Licenses
- [**Wallpaper Engine**](https://store.steampowered.com/app/431960/Wallpaper_Engine/) from Steam gives me the original idea to make this tool. Thanks to WE's author(s)!
- Main train of thought comes from https://github.com/ThomasHuai/Wallpaper , some codes are also copied from https://github.com/ThomasHuai/Wallpaper/blob/master/utils.cpp and https://github.com/ThomasHuai/Wallpaper/blob/master/wallpaper.cpp , thanks to [**ThomasHuai**](https://github.com/ThomasHuai)!
- Special thanks to [**wang bin**](https://github.com/wang-bin) for his awesome [QtAV](https://github.com/wang-bin/QtAV)!
- Most frameless window codes are copied from https://github.com/Bringer-of-Light/Qt-Nice-Frameless-Window , thanks to [Bringer-of-Light](https://github.com/Bringer-of-Light)
- Most style sheets of Preferences Dialog are copied from https://github.com/u8sand/Baka-MPlayer , thanks to [Daniel Clarke](https://github.com/u8sand)
- [src/images/bee.ico](/src/images/bee.ico): downloaded from https://www.easyicon.net/
- [src/images/minimize.ico](/src/images/minimize.ico), [src/images/maximize.ico](/src/images/maximize.ico), [src/images/restore.ico](/src/images/restore.ico), [src/images/close.ico](/src/images/close.ico): original svg files downloaded from http://www.iconfont.cn/ , and I converted them to ico files manually
- [src/images/checked.ico](/src/images/checked.ico), [src/images/down-arrow.ico](/src/images/down-arrow.ico), [src/images/selected.ico](/src/images/selected.ico): original svg files copied from https://github.com/u8sand/Baka-MPlayer , and I converted them to ico files manually
- Dynamic Desktop: [GPLv3](/LICENSE.md)
