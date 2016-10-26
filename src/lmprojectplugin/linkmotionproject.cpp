/*####################################################################
#
# This file is part of the LinkMotion Project plugin.
#
# License: Proprietary
# Author: Juhapekka Piiroinen <juhapekka.piiroinen@link-motion.com>
#
# All rights reserved.
# (C) 2016 Link Motion Oy
####################################################################*/
#include "linkmotionproject.h"
#include "linkmotionprojectplugin_constants.h"
#include "linkmotionkitmatcher.h"

#include <coreplugin/icontext.h>
#include <qtsupport/qtsupportconstants.h>
#include <coreplugin/documentmanager.h>
#include <coreplugin/featureprovider.h>
#include <qtsupport/qtkitinformation.h>

#include <QSettings>

using namespace LinkMotion;
using namespace LinkMotion::Internal;

LinkMotionProject::LinkMotionProject(LinkMotionProjectManager *manager, const QString &fileName) : QmakeProjectManager::QmakeProject(manager, fileName){
    qDebug() << Q_FUNC_INFO;
    setId(Constants::LINKMOTIONPROJECT_ID);
    setRequiredKitMatcher(LinkMotionKitMatcher());

    QSet<Core::Id> features;
    features << QtSupport::Constants::FEATURE_DESKTOP;
    setPreferredKitMatcher(QtSupport::QtKitInformation::qtVersionMatcher(features));
    setProjectContext(Core::Context(Constants::LINKMOTIONPROJECT_PROJECTCONTEXT));
}

