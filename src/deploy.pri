libs.path = $${BIN_DIR}
!CONFIG(static_ffmpeg) {
    isEmpty(ffmpeg_dir): ffmpeg_dir = $$[QT_INSTALL_BINS]
    exists("$${ffmpeg_dir}/avcodec-*.dll") {
        libs.files *= \
            $${ffmpeg_dir}/avcodec-*.dll \
            $${ffmpeg_dir}/avdevice-*.dll \
            $${ffmpeg_dir}/avfilter-*.dll \
            $${ffmpeg_dir}/avformat-*.dll \
            $${ffmpeg_dir}/avresample-*.dll \
            $${ffmpeg_dir}/avutil-*.dll \
            $${ffmpeg_dir}/ass.dll \
            $${ffmpeg_dir}/libass.dll \
            $${ffmpeg_dir}/OpenAL32*.dll \
            $${ffmpeg_dir}/postproc-*.dll \
            $${ffmpeg_dir}/swresample-*.dll \
            $${ffmpeg_dir}/swscale-*.dll
    } else {
        message("qmake can\'t find FFmpeg\'s run-time libraries in \"$${ffmpeg_dir}\".")
        message("You may have to copy them manually.")
        message("You can set the \"ffmpeg_dir\" variable to let qmake copy them automatically.")
    }
}
CONFIG(shared, static|shared) {
    exists("$$[QT_INSTALL_BINS]/QtAV*.dll") {
        CONFIG(debug, debug|release) {
            libs.files *= \
                $$[QT_INSTALL_BINS]/QtAVd?.dll \
                $$[QT_INSTALL_BINS]/QtAVWidgetsd?.dll
        } else:CONFIG(release, debug|release) {
            libs.files *= \
                $$[QT_INSTALL_BINS]/QtAV?.dll \
                $$[QT_INSTALL_BINS]/QtAVWidgets?.dll
        }
    } else {
        message("qmake can\'t find QtAV\'s run-time libraries in \"$$[QT_INSTALL_BINS]\".")
        message("You may have to copy them manually.")
        message("Currently(QtAV 1.12.0) they are QtAV[d]1.dll and QtAVWidgets[d]1.dll.")
    }
    isEmpty(windeployqt): windeployqt = $$[QT_INSTALL_BINS]/windeployqt.exe
    exists("$${windeployqt}") {
        target_file_name = $${TARGET}
        CONFIG(dll) {
            target_file_name = $${target_file_name}$${DD_MAJOR_VERSION}.dll
        } else {
            target_file_name = $${target_file_name}.exe
        }
        libs.commands *= $$quote(\"$${windeployqt}\" --plugindir \"$${BIN_DIR}\\plugins\" --no-translations --no-compiler-runtime --angle --no-opengl-sw -opengl --list source \"$${BIN_DIR}\\$${target_file_name}\")
    } else {
        message("It seems that there is no \"windeployqt.exe\" in \"$$[QT_INSTALL_BINS]\".")
        message("You may have to copy Qt run-time libraries manually and don\'t forget about the plugins.")
        message("Qt5Svg.dll, Qt5OpenGL.dll and plugins\\iconengines\\qsvgicon.dll are the necessary dlls you must copy.")
        message("d3dcompiler_XX.dll, libEGL.dll, libGLESv2.dll and opengl32sw.dll may be useful as well.")
    }
    target_arch = x86
    contains(QT_ARCH, x86_64): target_arch = x64
    !isEmpty(VC_REDIST_DIR):exists("$${VC_REDIST_DIR}") {
        isEmpty(VC_REDIST_VERSION): VC_REDIST_VERSION = 141
        vc_redist_dll_dir = Microsoft.VC$${VC_REDIST_VERSION}.CRT
        libs.commands *= $$quote(copy /y \"$${VC_REDIST_DIR}\\$${target_arch}\\$${vc_redist_dll_dir}\\msvcp*.dll\" \"$${BIN_DIR}\")
        libs.commands *= $$quote(copy /y \"$${VC_REDIST_DIR}\\$${target_arch}\\$${vc_redist_dll_dir}\\vcruntime*.dll\" \"$${BIN_DIR}\")
    }
    !isEmpty(WIN_SDK_REDIST_DIR):exists("$${WIN_SDK_REDIST_DIR}"): libs.commands *= $$quote(copy /y \"$${WIN_SDK_REDIST_DIR}\\ucrt\\DLLs\\$${target_arch}\\*.dll\" \"$${BIN_DIR}\")
    !isEmpty(libs.commands): libs.commands = $$join(libs.commands, $$escape_expand(\\n\\t))
} else:CONFIG(static, static|shared) {
    libs.files *= \
        $$[QT_INSTALL_BINS]/d3dcompiler_??.dll \
        $$[QT_INSTALL_BINS]/libEGL.dll \
        $$[QT_INSTALL_BINS]/libGLESv2.dll
}
!isEmpty(libs.files): INSTALLS *= libs
licenses.path = $${BIN_DIR}/licenses
licenses.files *= $${ROOT}/docs/licenses/*
!isEmpty(licenses.files): INSTALLS *= licenses