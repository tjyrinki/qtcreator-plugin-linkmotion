/*####################################################################
#
# This file is part of the LinkMotion Wizard plugin.
#
# License: Proprietary
# Author: Juhapekka Piiroinen <juhapekka.piiroinen@link-motion.com>
#
# All rights reserved.
# (C) 2016 Link Motion Oy
####################################################################*/
#ifndef LINKMOTIONPROJECTWIZARDFACTORY_H
#define LINKMOTIONPROJECTWIZARDFACTORY_H

#include <QObject>
#include <QDebug>
#include <projectexplorer/customwizard/customwizard.h>
#include "linkmotionprojectwizard.h"

namespace LinkMotion {
namespace Internal {

class LinkMotionProjectWizardFactory : public ProjectExplorer::ICustomWizardMetaFactory
{
public:
    LinkMotionProjectWizardFactory(const QString &klass, Core::IWizardFactory::WizardKind kind) : ICustomWizardMetaFactory(klass, kind) {
        qDebug() << Q_FUNC_INFO;
    }
    LinkMotionProjectWizardFactory(Core::IWizardFactory::WizardKind kind) : ICustomWizardMetaFactory(QString(), kind) {
        qDebug() << Q_FUNC_INFO;
    }
    ProjectExplorer::CustomWizard *create() const override {
        qDebug() << Q_FUNC_INFO;
        return new LinkMotionProjectWizard();
    }
};

}
}

#endif // LINKMOTIONPROJECTWIZARDFACTORY_H
