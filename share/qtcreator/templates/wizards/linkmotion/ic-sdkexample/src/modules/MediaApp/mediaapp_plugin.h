#ifndef MEDIAPP_PLUGIN_H
#define MEDIAPP_PLUGIN_H

#include <QQmlExtensionPlugin>

class MediaAppPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
    void registerTypes(const char *uri);
};

#endif // MEDIAAPP_PLUGIN_H
