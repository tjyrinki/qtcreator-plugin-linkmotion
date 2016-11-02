/*####################################################################
#
# This file is part of the LinkMotion Kit plugin.
#
# License: Proprietary
# Author: Juhapekka Piiroinen <juhapekka.piiroinen@link-motion.com>
#
# All rights reserved.
# (C) 2016 Link Motion Oy
####################################################################*/
#ifndef LINKMOTIONKITCONFIGURATION_H
#define LINKMOTIONKITCONFIGURATION_H

#include <QObject>

namespace LinkMotion {
namespace Internal {

class LinkMotionKitConfiguration : public QObject
{
    Q_OBJECT

public:
    explicit LinkMotionKitConfiguration(QObject *parent = 0);


public slots:
    void initialize();
    void autoConfigure();


};

}
}


#endif // LINKMOTIONKITCONFIGURATION_H
