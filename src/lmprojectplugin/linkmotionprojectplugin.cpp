#include "linkmotionprojectplugin.h"

#include "linkmotionprojectplugin_constants.h"
#include "linkmotionprojectmanager.h"
#include <utils/mimetypes/mimedatabase.h>

using namespace LinkMotion;
using namespace LinkMotion::Internal;

LinkMotionProjectPlugin::LinkMotionProjectPlugin()
{
    qDebug() << Q_FUNC_INFO;

}

LinkMotionProjectPlugin::~LinkMotionProjectPlugin()
{
    qDebug() << Q_FUNC_INFO;

}

bool LinkMotionProjectPlugin::initialize(const QStringList &arguments, QString *errorString)
{
    qDebug() << Q_FUNC_INFO;
    Q_UNUSED(arguments)
    Q_UNUSED(errorString)
    Utils::MimeDatabase::addMimeTypes(QLatin1String(Constants::LINKMOTION_MIMETYPE_XML));

    addAutoReleasedObject(new LinkMotionProjectManager);

/*
    addAutoReleasedObject(new LinkMotionDeployConfigurationFactory);
    addAutoReleasedObject(new LinkMotionDeployStepFactory);
*/
    return true;
}
