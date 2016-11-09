#ifndef CARAPP_PLUGIN_H
#define CARAPP_PLUGIN_H

#include <QQmlExtensionPlugin>

class CarAppPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
    void registerTypes(const char *uri);
};

#endif // CARAPP_PLUGIN_H
