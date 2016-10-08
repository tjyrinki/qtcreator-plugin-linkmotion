#ifndef LINKMOTIONWIZARDPLUGIN_H
#define LINKMOTIONWIZARDPLUGIN_H

#include <QtCore>
#include <extensionsystem/iplugin.h>
#include "linkmotionwizardplugin_global.h"

namespace LinkMotion {
namespace Internal {

class LINKMOTIONWIZARDSHARED_EXPORT LinkMotionWizardPlugin: public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "lmwizardplugin.json")

public:
    LinkMotionWizardPlugin();
    ~LinkMotionWizardPlugin();

    virtual bool initialize(const QStringList &arguments, QString *errorString) override;
    virtual void extensionsInitialized() override {}

};

}
}


#endif // LINKMOTIONWIZARDPLUGIN_H
