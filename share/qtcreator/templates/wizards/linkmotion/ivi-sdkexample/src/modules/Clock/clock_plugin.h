#ifndef CLOCK_PLUGIN_H
#define CLOCK_PLUGIN_H

#include <QQmlExtensionPlugin>

class ClockPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
    void registerTypes(const char *uri);
};

#endif // CLOCK_PLUGIN_H
