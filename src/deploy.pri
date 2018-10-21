!CONFIG(static_dd) {
    libs.path = $${BIN_DIR}
    isEmpty(windeployqt): windeployqt = $$[QT_INSTALL_BINS]/windeployqt.exe
    exists("$${windeployqt}") {
        bin = $${TARGET}
        CONFIG(dll) {
            bin = $$join(bin,,,$${DD_MAJOR_VERSION}.dll)
        } else {
            bin = $$join(bin,,,.exe)
        }
        libs.commands = $$quote(\"$${windeployqt}\" --plugindir \"$${BIN_DIR}/plugins\" --no-patchqt --no-translations --no-system-d3d-compiler --no-compiler-runtime --no-angle --no-opengl-sw --list source \"$${BIN_DIR}/$${bin}\")
        libs.commands = $$join(libs.commands, $$escape_expand(\\n\\t))
    }
    INSTALLS *= libs
}
