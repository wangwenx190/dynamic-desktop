CONFIG(build_installer) {
    isEmpty(iscc): iscc = $${inno_dir}/iscc.exe
    installer_arch = x86
    iscc_cmd = /Qp /DDDVersion=$${VERSION} /O$${BUILD_DIR}
    contains(QT_ARCH, x86_64) {
        installer_arch = x64
        iscc_cmd = $${iscc_cmd} /DWIN64
    }
    CONFIG(debug, debug|release): iscc_cmd = $${iscc_cmd} /D_DEBUG
    iscc_cmd = $${iscc_cmd} /Fdd_v$${VERSION}_$${installer_arch}
    inno.path = $${BIN_DIR}
    inno.commands = $$quote(\"$${iscc}\" $${iscc_cmd} \"$$PWD/installer/installer.iss\")
    inno.commands = $$join(inno.commands, $$escape_expand(\\n\\t))
    INSTALLS *= inno
}
