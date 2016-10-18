/*####################################################################
#
# This file is part of the LinkMotion Welcome plugin.
#
# License: Proprietary
# Author: Juhapekka Piiroinen <juhapekka.piiroinen@link-motion.com>
#
# All rights reserved.
# (C) 2016 Link Motion Oy
####################################################################*/

#ifndef LINKMOTIONWELCOMEMODE_H
#define LINKMOTIONWELCOMEMODE_H

#include <coreplugin/imode.h>
#include <QObject>
#include <QtQuickWidgets/QQuickWidget>

namespace LinkMotion {
namespace Internal {

class LinkMotionWelcomeMode : public Core::IMode
{
    Q_OBJECT
public:
    LinkMotionWelcomeMode();
    ~LinkMotionWelcomeMode();

protected slots:
    void onSceneGraphError(QQuickWindow::SceneGraphError,QString);

protected:
    QWidget* m_rootWidget;
    QQuickWidget* m_quickWidget;
};

}
}

#endif // LINKMOTIONWELCOMEMODE_H
