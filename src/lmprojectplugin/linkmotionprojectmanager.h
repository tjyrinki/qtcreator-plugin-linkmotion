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
#ifndef LINKMOTIONPROJECTMANAGER_H
#define LINKMOTIONPROJECTMANAGER_H

#include <QObject>
#include <projectexplorer/iprojectmanager.h>
#include <qmakeprojectmanager/qmakeprojectmanager.h>

namespace LinkMotion {
namespace Internal {

class LinkMotionProjectManager : public QmakeProjectManager::QmakeManager
{
    Q_OBJECT

public:
    LinkMotionProjectManager() : QmakeProjectManager::QmakeManager() {
        qDebug() << Q_FUNC_INFO;
    }
    ~LinkMotionProjectManager() {
        qDebug() << Q_FUNC_INFO;
    }
    virtual QString mimeType() const override;
    ProjectExplorer::Project* openProject(const QString &filePath, QString *errorString) override;
};

}
}

#endif // LINKMOTIONPROJECTMANAGER_H
