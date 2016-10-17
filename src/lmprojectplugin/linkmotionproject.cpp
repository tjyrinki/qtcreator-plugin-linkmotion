#include "linkmotionproject.h"
#include "linkmotionprojectplugin_constants.h"
#include "linkmotionkitmatcher.h"

#include <coreplugin/icontext.h>
#include <qtsupport/qtsupportconstants.h>
#include <coreplugin/documentmanager.h>
#include <qtsupport/qtkitinformation.h>

#include <QSettings>

using namespace LinkMotion;
using namespace LinkMotion::Internal;

LinkMotionProject::LinkMotionProject(LinkMotionProjectManager *manager, const QString &fileName) : QmakeProjectManager::QmakeProject(manager, fileName){
    qDebug() << Q_FUNC_INFO;
    setId(Constants::LINKMOTIONPROJECT_ID);
    setRequiredKitMatcher(LinkMotionKitMatcher());
    setPreferredKitMatcher(QtSupport::QtKitInformation::qtVersionMatcher(Core::FeatureSet(QtSupport::Constants::FEATURE_DESKTOP)));
    setProjectContext(Core::Context(Constants::LINKMOTIONPROJECT_PROJECTCONTEXT));
}

