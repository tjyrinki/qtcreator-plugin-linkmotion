#include "linkmotionwizardplugin.h"

using namespace LinkMotion;
using namespace LinkMotion::Internal;

LinkMotionWizardPlugin::LinkMotionWizardPlugin()
{
    qDebug() << Q_FUNC_INFO;

}

LinkMotionWizardPlugin::~LinkMotionWizardPlugin()
{
    qDebug() << Q_FUNC_INFO;

}

bool LinkMotionWizardPlugin::initialize(const QStringList &arguments, QString *errorString)
{
    qDebug() << Q_FUNC_INFO;
    Q_UNUSED(arguments)
    Q_UNUSED(errorString)

/*
    addAutoReleasedObject(new LinkMotionDeployConfigurationFactory);
    addAutoReleasedObject(new LinkMotionDeployStepFactory);
*/
    return true;
}
