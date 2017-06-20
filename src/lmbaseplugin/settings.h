/*
 * Copyright 2013 - 2016 Canonical Ltd.
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
#ifndef LM_INTERNAL_SETTINGS_H
#define LM_INTERNAL_SETTINGS_H

#include <utils/fileutils.h>

#include <QObject>
#include <QVariantMap>

namespace Utils { class PersistentSettingsWriter; }

namespace LmBase {
namespace Internal {

class Settings : public QObject
{
    Q_OBJECT
public:

    struct DeviceConnectivity {
        QString user = QStringLiteral("phablet");
        QString ip   = QStringLiteral("127.0.0.1");
        int sshPort  = 2222;
    };

    struct ProjectDefaults {
        bool reviewErrorsAsWarnings = false;
        bool enableDebugHelper      = true;
        bool uninstallAppsByDefault = true;
        bool overrideAppsByDefault  = false;
    };

    struct TargetSettings {
        bool useLocalMirror = false;
    };

    explicit Settings();
    virtual ~Settings();

    void restoreSettings ();
    static void flushSettings ();

    static Utils::FileName settingsPath ();

    static DeviceConnectivity deviceConnectivity ();
    static void setDeviceConnectivity (const DeviceConnectivity &settings);

    static ProjectDefaults projectDefaults ();
    static void setProjectDefaults (const ProjectDefaults &settings);

    static TargetSettings chrootSettings ();
    static void setChrootSettings (const TargetSettings &settings);

    static bool deviceAutoToggle ();
    static void setDeviceAutoToggle (const bool set);

    static bool askForContainerSetup ();
    static void setAskForContainerSetup (const bool set);

private:
    static Settings *m_instance;
    QVariantMap m_settings;
    Utils::PersistentSettingsWriter *m_writer = 0;

};

} // namespace Internal
} // namespace LmBase

#endif // LM_INTERNAL_SETTINGS_H
