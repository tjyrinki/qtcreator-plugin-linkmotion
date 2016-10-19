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
#ifndef LINKMOTIONPROJECTWIZARD_H
#define LINKMOTIONPROJECTWIZARD_H

#include <QObject>
#include <QDebug>
#include <projectexplorer/customwizard/customwizard.h>

namespace LinkMotion {
namespace Internal {

class LinkMotionProjectWizard : public ProjectExplorer::CustomProjectWizard
{
    Q_OBJECT

public:
    LinkMotionProjectWizard() : CustomProjectWizard() {
        qDebug() << Q_FUNC_INFO;
    }

protected:
    virtual Core::GeneratedFiles generateFiles(const QWizard *w, QString *errorMessage) const override;
    Core::BaseFileWizard *create(QWidget *parent, const Core::WizardDialogParameters &wizardDialogParameters) const override;
    bool postGenerateFiles(const QWizard *, const Core::GeneratedFiles &l, QString *errorMessage) const override;

};

}
}

#endif // LINKMOTIONPROJECTWIZARD_H
