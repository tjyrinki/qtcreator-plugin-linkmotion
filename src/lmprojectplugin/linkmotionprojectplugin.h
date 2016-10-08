#ifndef LINKMOTIONPROJECTPLUGIN_H
#define LINKMOTIONPROJECTPLUGIN_H

#include <QtCore>
#include <extensionsystem/iplugin.h>
#include "linkmotionprojectplugin_global.h"

namespace LinkMotion {
namespace Internal {

class LINKMOTIONPROJECTSHARED_EXPORT LinkMotionProjectPlugin: public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "lmprojectplugin.json")

public:
    LinkMotionProjectPlugin();
    ~LinkMotionProjectPlugin();

    virtual bool initialize(const QStringList &arguments, QString *errorString) override;
    virtual void extensionsInitialized() override {}

};

}
}


#endif // LINKMOTIONPROJECTPLUGIN_H
