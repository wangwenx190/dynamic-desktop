isEmpty(lupdate): lupdate = $${qttools_dir}/lupdate.exe
isEmpty(lrelease): lrelease = $${qttools_dir}/lrelease.exe
lrelease_params = -nounfinished -removeidentical
CONFIG(small): lrelease_params = $${lrelease_params} -compress
exists("$${lupdate}") {
    system("$${lupdate} -no-obsolete $${_PRO_FILE_}")
    system("$${lrelease} $${lrelease_params} $${_PRO_FILE_}")
}
CONFIG(static_dd) {
    RESOURCES *= $$PWD/i18n.qrc
} else {
    translations.path = $${BIN_DIR}/translations
    translations.files = ../resources/translations/*.qm
    INSTALLS *= translations
}
