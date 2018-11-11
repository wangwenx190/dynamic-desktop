!win32: error("This project only supports Win32 platform!")
!qtHaveModule(gui): error("qmake can\'t find \"gui\" module. Compilation aborted.")
!qtHaveModule(widgets): error("qmake can\'t find \"widgets\" module. Compilation aborted.")
!qtHaveModule(network): error("qmake can\'t find \"network\" module. Compilation aborted.")
!qtHaveModule(opengl): error("qmake can\'t find \"opengl\" module. Compilation aborted.")
!qtHaveModule(svg):!CONFIG(no_svg) {
    message("qmake can\'t find \"svg\" module. QtSVG support is disabled now.")
    CONFIG *= no_svg
}
!qtHaveModule(winextras):!CONFIG(no_win_extras) {
    message("qmake can\'t find \"winextras\" module. QtWinExtras support is disabled now.")
    CONFIG *= no_win_extras
}
TEMPLATE = subdirs
CONFIG *= ordered
SUBDIRS *= \
    src/ddutils \
    src/ddservice \
    src/ddmain
