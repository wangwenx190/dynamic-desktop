isEmpty(ROOT): ROOT = $$PWD/..
isEmpty(BUILD_DIR): BUILD_DIR = $${ROOT}/build
isEmpty(BIN_DIR): BIN_DIR = $${BUILD_DIR}/bin
contains(QT_ARCH, x86_64): BIN_DIR = $$join(BIN_DIR,,,64)
isEmpty(qttools_dir): qttools_dir = $$[QT_INSTALL_BINS]
isEmpty(ffmpeg_dir): ffmpeg_dir = $$[QT_INSTALL_BINS]
isEmpty(upx_dir): upx_dir = $$[QT_INSTALL_BINS]
isEmpty(inno_dir): inno_dir = $$[QT_INSTALL_BINS]
