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

#ifndef UBUNTULOCALRUNCONTROLFACTORY_H
#define UBUNTULOCALRUNCONTROLFACTORY_H

#include <projectexplorer/runconfiguration.h>
#include <utils/environment.h>

namespace LmBase {
namespace Internal {

class UbuntuLocalRunControlFactory : public ProjectExplorer::IRunControlFactory
{
    Q_OBJECT
public:
    explicit UbuntuLocalRunControlFactory() {}
    virtual ~UbuntuLocalRunControlFactory() {}

    bool canRun(ProjectExplorer::RunConfiguration *runConfiguration, Core::Id mode) const override;
    ProjectExplorer::RunControl *create(ProjectExplorer::RunConfiguration *runConfiguration,
                                        Core::Id mode, QString *errorMessage) override;
    QString displayName() const;

};

} //namespace Internal
} //namespace LmBase
#endif // UBUNTULOCALRUNCONTROLFACTORY_H
