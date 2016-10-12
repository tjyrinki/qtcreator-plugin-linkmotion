TEMPLATE = app
QT += qml quick qmltest
CONFIG += no_keywords

check.target = check
check.commands = $$shadowed($$_PRO_FILE_PWD_)/$${TARGET} -import $${IMPORTPATH}

check-xml.target = check-xml
check-xml.commands = mkdir -p ../../../results/; $$shadowed($$_PRO_FILE_PWD_)/$${TARGET} -import $${IMPORTPATH} -o ../../../results/$${TARGET}.xml,xunitxml

QMAKE_EXTRA_TARGETS += check-xml check

