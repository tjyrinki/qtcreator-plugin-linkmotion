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
#ifndef LINKMOTIONPROJECTWIZARDDIALOG_H
#define LINKMOTIONPROJECTWIZARDDIALOG_H

#include <QObject>
#include <projectexplorer/baseprojectwizarddialog.h>
#include <projectexplorer/targetsetuppage.h>

class LinkMotionProjectWizardDialog : public ProjectExplorer::BaseProjectWizardDialog
{
    Q_OBJECT

public:
    LinkMotionProjectWizardDialog(const Core::BaseFileWizardFactory *factory, QWidget *parent, const Core::WizardDialogParameters &parameters);
    ~LinkMotionProjectWizardDialog();

    bool writeUserFile(const QString &projectFileName) const;
    QList<Core::Id> selectedKits() const;

protected:
    ProjectExplorer::TargetSetupPage* m_targetSetupPage;
};

#endif // LINKMOTIONPROJECTWIZARDDIALOG_H
