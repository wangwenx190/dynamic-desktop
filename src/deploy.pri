$${TARGET}_libs.path = $${BIN_DIR}
!CONFIG(static_ffmpeg) {
    isEmpty(ffmpeg_dir): ffmpeg_dir = $${ROOT}/ffmpeg
    isEmpty(ffmpeg_bin_dir): ffmpeg_bin_dir = $${ffmpeg_dir}/bin
    !exists($${ffmpeg_bin_dir}/avcodec*.dll) {
        contains(QT_ARCH, x86_64): ffmpeg_bin_dir = $${ffmpeg_bin_dir}/x64
        else: ffmpeg_bin_dir = $${ffmpeg_bin_dir}/x86
    }
    exists("$${ffmpeg_bin_dir}/avcodec*.dll") {
        $${TARGET}_libs.files *= \
            $${ffmpeg_bin_dir}/avcodec*.dll \
            $${ffmpeg_bin_dir}/avdevice*.dll \
            $${ffmpeg_bin_dir}/avfilter*.dll \
            $${ffmpeg_bin_dir}/avformat*.dll \
            $${ffmpeg_bin_dir}/avutil*.dll \
            $${ffmpeg_bin_dir}/postproc*.dll \
            $${ffmpeg_bin_dir}/swresample*.dll \
            $${ffmpeg_bin_dir}/swscale*.dll
        CONFIG(enable_avresample): $${TARGET}_libs.files *= $${ffmpeg_bin_dir}/avresample*.dll
        CONFIG(enable_libass):!CONFIG(static_libass): $${TARGET}_libs.files *= $${ffmpeg_bin_dir}/ass.dll $${ffmpeg_bin_dir}/libass.dll
        CONFIG(enable_openal):!CONFIG(static_openal): $${TARGET}_libs.files *= $${ffmpeg_bin_dir}/OpenAL32.dll $${ffmpeg_bin_dir}/soft_oal.dll
    }
}
CONFIG(shared, static|shared):CONFIG(qt) {
    isEmpty(windeployqt): windeployqt = $$[QT_INSTALL_BINS]/windeployqt
    exists("$${windeployqt}.exe") {
        target_file_name = $${TARGET}
        CONFIG(dll) {
            exists($${BIN_DIR}/$${target_file_name}$${DD_MAJOR_VERSION}.dll): target_file_name = $${target_file_name}$${DD_MAJOR_VERSION}.dll
            else: target_file_name = $${target_file_name}.dll
        } else {
            target_file_name = $${target_file_name}.exe
        }
        windeployqt_command = --plugindir \"$${BIN_DIR}\\plugins\" --no-translations --no-compiler-runtime -opengl --list source
        CONFIG(no_llvmpipe)|CONFIG(no_mesa): windeployqt_command = $${windeployqt_command} --no-opengl-sw
        CONFIG(no_angle): windeployqt_command = $${windeployqt_command} --no-system-d3d-compiler --no-angle
        CONFIG(no_svg): windeployqt_command = $${windeployqt_command} --no-svg
        $${TARGET}_libs.commands *= $$quote(\"$${windeployqt}\" $${windeployqt_command} \"$${BIN_DIR}\\$${target_file_name}\")
    } else {
        message("It seems that there is no \"windeployqt\" in \"$$[QT_INSTALL_BINS]\".")
        message("You may have to copy Qt run-time libraries manually and don\'t forget about the plugins.")
        message("Qt5Svg.dll, Qt5OpenGL.dll and plugins\\iconengines\\qsvgicon.dll are the necessary dlls you must copy.")
        message("d3dcompiler_XX.dll, libEGL.dll, libGLESv2.dll and opengl32sw.dll may be useful as well.")
    }
} else:CONFIG(static, static|shared) {
    $${TARGET}_libs.files *= \
        $$[QT_INSTALL_BINS]/d3dcompiler_??.dll \
        $$[QT_INSTALL_BINS]/libEGL.dll \
        $$[QT_INSTALL_BINS]/libGLESv2.dll
}
CONFIG(copy_msvcrt_dlls) {
    target_arch = x86
    contains(QT_ARCH, x86_64): target_arch = x64
    !isEmpty(VC_REDIST_DIR):exists("$${VC_REDIST_DIR}") {
        isEmpty(VC_REDIST_VERSION): VC_REDIST_VERSION = 141
        vc_redist_dll_dir = Microsoft.VC$${VC_REDIST_VERSION}.CRT
        $${TARGET}_libs.commands *= $$quote(copy /y \"$${VC_REDIST_DIR}\\$${target_arch}\\$${vc_redist_dll_dir}\\msvcp*.dll\" \"$${BIN_DIR}\")
        $${TARGET}_libs.commands *= $$quote(copy /y \"$${VC_REDIST_DIR}\\$${target_arch}\\$${vc_redist_dll_dir}\\vcruntime*.dll\" \"$${BIN_DIR}\")
    }
    !isEmpty(WIN_SDK_REDIST_DIR):exists("$${WIN_SDK_REDIST_DIR}"): $${TARGET}_libs.commands *= $$quote(copy /y \"$${WIN_SDK_REDIST_DIR}\\ucrt\\DLLs\\$${target_arch}\\*.dll\" \"$${BIN_DIR}\")
}
!isEmpty($${TARGET}_libs.commands)|!isEmpty($${TARGET}_libs.files) {
    !isEmpty($${TARGET}_libs.commands): $${TARGET}_libs.commands = $$join($${TARGET}_libs.commands, $$escape_expand(\\n\\t))
    INSTALLS *= $${TARGET}_libs
    !CONFIG(static_ffmpeg):isEmpty($${TARGET}_libs.files) {
        message("qmake can\'t find FFmpeg\'s run-time libraries in \"$${ffmpeg_bin_dir}\".")
        message("You may have to copy them manually.")
        message("You can set the \"ffmpeg_dir\" variable to let qmake copy them automatically.")
    }
}
!CONFIG(no_licenses) {
    licenses.path = $${BIN_DIR}/licenses
    licenses.files *= $${ROOT}/docs/licenses/*
    !isEmpty(licenses.files): INSTALLS *= licenses
}
