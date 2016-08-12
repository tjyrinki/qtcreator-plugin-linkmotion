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

#ifndef LINKMOTIONWELCOMEPAGE_H
#define LINKMOTIONWELCOMEPAGE_H

#include <coreplugin/iwelcomepage.h>
#include <QObject>

namespace LinkMotion {
namespace Internal {

class LinkMotionWelcomePage : public Core::IWelcomePage
{
    Q_OBJECT

public:
    // IWelcomePage interface
    virtual QUrl pageLocation() const;
    virtual QString title() const;
    virtual int priority() const;
    virtual void facilitateQml(QQmlEngine *engine);
    virtual Core::Id id() const;

public slots:

};

}
}

#endif // LINKMOTIONWELCOMEPAGE_H
