CONFIG(upx) {
    isEmpty(upx): upx = $${upx_dir}/upx.exe
    upx.path = $${BIN_DIR}
    upx.commands += $$quote(\"$${upx}\" --force --ultra-brute \"$${BIN_DIR}/*.exe\")
    upx.commands += $$quote(\"$${upx}\" --force --ultra-brute \"$${BIN_DIR}/*.dll\")
    upx.commands = $$join(upx.commands, $$escape_expand(\\n\\t))
    INSTALLS += upx
}
