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

    virtual QString defaultPath() const override;
    virtual QString suggestedFileName() const override;

    virtual bool isModified() const override;
    virtual bool isSaveAsAllowed() const override;

    virtual ReloadBehavior reloadBehavior(ChangeTrigger state, ChangeType type) const override;
    virtual bool reload(QString *errorString, ReloadFlag flag, ChangeType) override;

private:
    LinkMotionProject *m_project;
    QString m_filename;
};

}
}

#endif // LINKMOTIONPROJECTFILE_H
