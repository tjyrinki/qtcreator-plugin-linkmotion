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
#ifndef LINKMOTIONPROJECTFILE_H
#define LINKMOTIONPROJECTFILE_H

#include <QObject>
#include <coreplugin/idocument.h>

#include "linkmotionproject.h"

namespace LinkMotion {
namespace Internal {

class LinkMotionProject;

class LinkMotionProjectFile : public Core::IDocument
{
public:
    LinkMotionProjectFile(LinkMotionProject *project, QString fileName);

    virtual bool save(QString *errorString, const QString &fileName = QString(), bool autoSave = false) override;

    virtual QString defaultPath() const;
    virtual QString suggestedFileName() const ;

    virtual bool isModified() const override;
    virtual bool isSaveAsAllowed() const override;

    virtual ReloadBehavior reloadBehavior(ChangeTrigger state, ChangeType type) const override;
    virtual bool reload(QString *errorString, ReloadFlag flag, ChangeType) override;

private:
    LinkMotionProject *m_project;
    QString m_filename;
    LinkMotionProjectFile* m_fileInfo;
};

}
}

#endif // LINKMOTIONPROJECTFILE_H
