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
#include "linkmotionprojectwizard.h"

#include "linkmotionprojectwizarddialog.h"

using namespace LinkMotion;
using namespace LinkMotion::Internal;

Core::GeneratedFiles LinkMotionProjectWizard::generateFiles(const QWizard *w, QString *errorMessage) const {
    qDebug() << Q_FUNC_INFO;
    return ProjectExplorer::CustomProjectWizard::generateFiles(w,errorMessage);
}

Core::BaseFileWizard *LinkMotionProjectWizard::create(QWidget *parent, const Core::WizardDialogParameters &wizardDialogParameters) const {
    qDebug() << Q_FUNC_INFO;
    LinkMotionProjectWizardDialog *projectDialog = new LinkMotionProjectWizardDialog(this, parent, wizardDialogParameters);
    initProjectWizardDialog(projectDialog, wizardDialogParameters.defaultPath(), projectDialog->extensionPages());
    return projectDialog;
}

bool LinkMotionProjectWizard::postGenerateFiles(const QWizard *w, const Core::GeneratedFiles &l, QString *errorMessage) const {
    qDebug() << Q_FUNC_INFO;
    const LinkMotionProjectWizardDialog *dialog = qobject_cast<const LinkMotionProjectWizardDialog *>(w);

    foreach (const Core::GeneratedFile &file, l) {
        if (file.attributes() & Core::GeneratedFile::OpenProjectAttribute) {
            dialog->writeUserFile(file.path());
            break;
        }
    }

    return ProjectExplorer::CustomProjectWizard::postGenerateOpen(l ,errorMessage);
}
