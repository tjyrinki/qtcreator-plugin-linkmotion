TEMPLATE = subdirs
SUBDIRS = 

QMAKE_EXTRA_TARGETS += check
check.depends = ../../../src/app/app
check.commands += ./run-tests

OTHER_FILES += run-tests $$system(find . |grep rb$)
