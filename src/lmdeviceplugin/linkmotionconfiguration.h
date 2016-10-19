/*####################################################################
#
# This file is part of the LinkMotion Device plugin.
#
# License: Proprietary
# Author: Juhapekka Piiroinen <juhapekka.piiroinen@link-motion.com>
#
# All rights reserved.
# (C) 2016 Link Motion Oy
####################################################################*/
#ifndef LINKMOTIONCONFIGURATION_H
#define LINKMOTIONCONFIGURATION_H

#include <QObject>

namespace LinkMotion {
namespace Internal  {

class LinkMotionConfiguration : public QObject
{
    Q_OBJECT

public:
    static QObject *instance();
    static void initialize();

private:
    LinkMotionConfiguration(QObject *parent);
    void updateDevices();
};

}
}

#endif // LINKMOTIONCONFIGURATION_H
