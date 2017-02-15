/*####################################################################
#
# This file is part of the LinkMotion Kit plugin.
#
# License: Proprietary
#
# All rights reserved.
# (C) 2017 Link Motion Oy
####################################################################*/

#ifndef LINKMOTIONOSINFORMATION_H
#define LINKMOTIONOSINFORMATION_H

#include <projectexplorer/kitmanager.h>

namespace LinkMotion {
namespace Internal {

class LinkMotionOsKitInformation: public ProjectExplorer::KitInformation
{
    Q_OBJECT

public:
    LinkMotionOsKitInformation();

    QVariant defaultValue(const ProjectExplorer::Kit *k) const override;
    QList<ProjectExplorer::Task> validate(const ProjectExplorer::Kit *k) const override;
    ProjectExplorer::KitConfigWidget *createConfigWidget(ProjectExplorer::Kit *k) const override;
    KitInformation::ItemList toUserOutput(const ProjectExplorer::Kit *k) const override;

    void addToEnvironment(const ProjectExplorer::Kit *k, Utils::Environment &env) const override;

    static Core::Id id();
    static QString os(const ProjectExplorer::Kit *k);

};

} // namespace Internal
} // namespace LinkMotion

#endif // LINKMOTIONOSINFORMATION_H
