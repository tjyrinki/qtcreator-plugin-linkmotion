#ifndef LINKMOTIONDEVICEFACTORY_H
#define LINKMOTIONDEVICEFACTORY_H

#include <projectexplorer/devicesupport/idevicefactory.h>

namespace LinkMotion {
namespace Internal {

class LinkMotionDeviceFactory : public ProjectExplorer::IDeviceFactory
{
    Q_OBJECT

public:
    LinkMotionDeviceFactory();

    QString displayNameForId(Core::Id type) const override;
    QList<Core::Id> availableCreationIds() const override;

    bool canCreate() const override;
    ProjectExplorer::IDevice::Ptr create(Core::Id id) const override;
    bool canRestore(const QVariantMap &map) const override;
    ProjectExplorer::IDevice::Ptr restore(const QVariantMap &map) const override;

};

}
}

#endif // LINKMOTIONDEVICEFACTORY_H

