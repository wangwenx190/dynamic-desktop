[![Build status](https://ci.appveyor.com/api/projects/status/7n23thxiormp6oar/branch/develop?svg=true)](https://ci.appveyor.com/project/wangwenx190/dynamic-desktop/branch/develop)
[![Code quality](https://api.codacy.com/project/badge/Grade/b41d1d384abe4f81a9613945cf654ff6)](https://www.codacy.com/app/wangwenx190/dynamic-desktop?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=wangwenx190/dynamic-desktop&amp;utm_campaign=Badge_Grade)
[![Bugs](https://img.shields.io/github/issues/wangwenx190/dynamic-desktop/bug.svg)](https://github.com/wangwenx190/dynamic-desktop/issues?utf8=✓&q=is%3Aissue+is%3Aopen+label%3Abug)
[![Latest release](https://img.shields.io/github/downloads/wangwenx190/dynamic-desktop/total.svg)](https://github.com/wangwenx190/dynamic-desktop/releases/latest)

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
- Currently, hardware decoding only supports traditional video formats such as AVC/H.264, so please do not attempt to play HEVC videos if you are using hardware decoding.

## Compilation
- Clone or download(don't forget submodules) this repository
   ```bat
   git clone --recursive https://github.com/wangwenx190/dynamic-desktop.git
   ```
- Download [Qt5](http://download.qt.io/archive/qt/) and install it.
- Download [QtAV](https://github.com/wang-bin/QtAV)'s prebuilt SDK and install it, or build yourself.
- Download [FFmpeg](https://github.com/wang-bin/avbuild)'s prebuilt binaries or build yourself.
- Open "dynamic-desktop.pro" and start compiling or call "build.bat". But remember to call "vcvarsall.bat" and add Qt's directories to your path variables before calling "build.bat".

**IMPORTANT NOTE**

You can also use Intel C++ Compiler(ICC), Clang or MinGW to compile it, just remember to set up environment variables. Cross compile on other platforms may also work, but it will never run on platforms other than MS Windows.

## Contributing
See [CONTRIBUTING.md](/CONTRIBUTING.md) for more information.

## Licenses
- [Wallpaper](/src/3rdparty/wallpaper): https://github.com/ThomasHuai/Wallpaper , Apache License 2.0, thanks to [**ThomasHuai**](https://github.com/ThomasHuai)!
- [QtAV](https://github.com/wang-bin/QtAV): GPLv3/LGPLv2.1, thanks to [**wang bin**](https://github.com/wang-bin)!
- [Qt Nice Frameless Window](/src/3rdparty/framelesswindow): https://github.com/Bringer-of-Light/Qt-Nice-Frameless-Window , MIT, thanks to [Bringer of Light](https://github.com/Bringer-of-Light)!
- [Qt](http://download.qt.io/archive/qt/) and [Qt Service](/src/3rdparty/qtservice): https://github.com/qtproject/qt-solutions , GPLv3/LGPLv2.1, thanks to [the Qt Company](https://www.qt.io/)!
- [QSimpleUpdater](/src/3rdparty/qsimpleupdater): https://github.com/alex-spataru/QSimpleUpdater , DON'T BE A DICK PUBLIC LICENSE, thanks to [Alex Spataru](https://github.com/alex-spataru)!
- [src/dd/stylesheets/Default.css](/src/dd/stylesheets/Default.css): copied from https://github.com/u8sand/Baka-MPlayer , GPLv2, thanks to [Daniel Clarke](https://github.com/u8sand)!
- [src/dd/stylesheets/Greenery.css](/src/dd/stylesheets/Greenery.css), [src/dd/stylesheets/Marsala.css](/src/dd/stylesheets/Marsala.css), [src/dd/stylesheets/Rose&nbsp;Quartz&nbsp;&amp;&nbsp;Serenity.css](/src/dd/stylesheets/Rose%20Quartz%20&%20Serenity.css), [src/dd/stylesheets/Ultra&nbsp;Violet.css](/src/dd/stylesheets/Ultra%20Violet.css): created by [Yuriko](https://github.com/GA-1101), thanks a lot!
- [src/dd/images/bee.ico](/src/dd/images/bee.ico), [src/svc/images/gear.ico](/src/svc/images/gear.ico), [src/udt/images/refresh.ico](/src/udt/images/refresh.ico): downloaded from https://www.easyicon.net/
- [src/dd/images/minimize.ico](/src/dd/images/minimize.ico), [src/dd/images/maximize.ico](/src/dd/images/maximize.ico), [src/dd/images/restore.ico](/src/dd/images/restore.ico), [src/dd/images/close.ico](/src/dd/images/close.ico): original svg files downloaded from http://www.iconfont.cn/ and I converted them to ico files manually
- [src/dd/images/checked.ico](/src/dd/images/checked.ico), [src/dd/images/down-arrow.ico](/src/dd/images/down-arrow.ico), [src/dd/images/selected.ico](/src/dd/images/selected.ico): original svg files copied from https://github.com/u8sand/Baka-MPlayer and I converted them to ico files manually
- Dynamic Desktop: [GPLv3](/LICENSE.md)
