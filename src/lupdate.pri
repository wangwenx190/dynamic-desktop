qtPrepareTool(QMAKE_LUPDATE, lupdate)

lupdate.name = lupdate
lupdate.input = TRANSLATIONS EXTRA_TRANSLATIONS
lupdate.commands = $$QMAKE_LUPDATE ${QMAKE_FILE_IN} $$QMAKE_LUPDATE_FLAGS
silent: lupdate.commands = @echo lupdate ${QMAKE_FILE_IN} && $$lupdate.commands
lupdate.CONFIG = no_link target_predeps no_clean
QMAKE_EXTRA_COMPILERS *= lupdate
