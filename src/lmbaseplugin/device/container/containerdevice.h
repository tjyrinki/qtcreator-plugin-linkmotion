/*
 * Copyright 2016 Canonical Ltd.
 * Copyright 2017 Link Motion Oy
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; version 2.1.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Benjamin Zeller <benjamin.zeller@link-motion.com>
 */
#ifndef LM_INTERNAL_CONTAINERDEVICE_H
#define LM_INTERNAL_CONTAINERDEVICE_H

#include <remotelinux/linuxdevice.h>


namespace LmBase {
namespace Internal {

class ContainerDevicePrivate;
class ContainerDevice : public RemoteLinux::LinuxDevice
{
public:
    typedef QSharedPointer<ContainerDevice> Ptr;
    typedef QSharedPointer<const ContainerDevice> ConstPtr;

    static Ptr create(Core::Id type, Core::Id id);

    ~ContainerDevice ();

    static Core::Id createIdForContainer(const QString &name);
    QString containerName() const;

    // IDevice interface
    virtual ProjectExplorer::IDeviceWidget *createWidget() override;
    virtual QList<Core::Id> actionIds() const override;
    virtual void executeAction(Core::Id actionId, QWidget *parent) override;
    virtual IDevice::Ptr clone() const override;
    virtual QString displayNameForActionId(Core::Id actionId) const override;
    virtual QString displayType() const override;
    virtual ProjectExplorer::DeviceProcess *createProcess(QObject *parent) const override;

protected:
    ContainerDevice(Core::Id type, Core::Id id);
    ContainerDevice(const ContainerDevice &other);

private:
    Q_DECLARE_PRIVATE(ContainerDevice)
    ContainerDevicePrivate *d_ptr;
};

} // namespace Internal
} // namespace LmBase

#endif // LM_INTERNAL_CONTAINERDEVICE_H
