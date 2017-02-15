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
#ifndef LINKMOTIONDEVICE_H
#define LINKMOTIONDEVICE_H

#include <QObject>
#include <projectexplorer/devicesupport/idevice.h>

namespace LinkMotion {
namespace Internal {

class LinkMotionDevice : public ProjectExplorer::IDevice
{
public:

    virtual QString displayType() const  override;
    virtual ProjectExplorer::IDeviceWidget *createWidget() override;
    virtual QList<Core::Id> actionIds() const  override;
    virtual QString displayNameForActionId(Core::Id actionId) const override;
    virtual void executeAction(Core::Id actionId, QWidget *parent = 0) override;
    virtual ProjectExplorer::DeviceProcessSignalOperation::Ptr signalOperation() const override;
    virtual Ptr clone() const override;

    static QString name();
    LinkMotionDevice(Core::Id id);

protected:
    friend class LinkMotionDeviceFactory;

    LinkMotionDevice();
    LinkMotionDevice(const LinkMotionDevice &other);

};

}
}

#endif // LINKMOTIONDEVICE_H
