qtPrepareTool(QMAKE_LUPDATE, lupdate)

isEmpty(LUPDATE_DIR): LUPDATE_DIR = $$PWD/resources/translations

lupdate.name = lupdate
lupdate.input = $$_PRO_FILE_
lupdate.output = $$LUPDATE_DIR/${QMAKE_FILE_IN_BASE}.ts
lupdate.commands = $$QMAKE_LUPDATE $$QMAKE_LUPDATE_FLAGS ${QMAKE_FILE_IN}
silent: lupdate.commands = @echo lupdate ${QMAKE_FILE_IN} && $$lupdate.commands
lupdate.CONFIG = no_link target_predeps no_clean
QMAKE_EXTRA_COMPILERS *= lupdate
