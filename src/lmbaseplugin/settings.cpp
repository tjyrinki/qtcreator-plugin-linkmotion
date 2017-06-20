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
#include "settings.h"
#include "lmbaseplugin_constants.h"

#include <coreplugin/icore.h>
#include <utils/fileutils.h>
#include <utils/persistentsettings.h>
#include <utils/qtcassert.h>

#include <QDir>
#include <QSettings>
#include <QString>
#include <QVariant>
#include <QDebug>
#include <QProcess>

namespace {
static const char LMSDK_DATA_KEY[] = "LinkMotionSdk.";
static const char LMSDK_FILE_VERSION_KEY[] = "Version";
static const char LMSDK_FILENAME[] = "/qtcreator/lm-sdk/config.xml";

static const bool DEFAULT_DEVICES_AUTOTOGGLE = true;
static const bool DEFAULT_ASK_FOR_CONTAINER_SETUP = true;

static const char KEY_USERNAME[] = "DeviceConnectivity.Username";
static const char KEY_IP[] = "DeviceConnectivity.IP";
static const char KEY_SSH[] = "DeviceConnectivity.SSH";
static const char KEY_AUTOTOGGLE[] = "Devices.Auto_Toggle";
static const char KEY_CHROOT_USE_LOCAL_MIRROR[] = "Target.Use_Local_Mirror";
static const char KEY_TREAT_REVIEW_ERRORS_AS_WARNINGS[] = "ProjectDefaults.Treat_Review_Warnings_As_Errors";
static const char KEY_ENABLE_DEBUG_HELPER_DEFAULT[] = "ProjectDefaults.Enable_Debug_Helper_By_Default";
static const char KEY_UNINSTALL_APPS_FROM_DEVICE_DEFAULT[] = "ProjectDefaults.Uninstall_Apps_From_Device_By_Default";
static const char KEY_OVERRIDE_APPS_BY_DEFAULT[] = "ProjectDefaults.Override_Apps_By_Default";
static const char KEY_ASK_FOR_CONTAINER_SETUP[] = "BasicSettings.AskForContainerSetup";
}

using namespace Utils;

static FileName settingsFileName(const QString &path, QSettings::Scope scope = QSettings::UserScope)
{
    QFileInfo settingsLocation(Core::ICore::settings(scope)->fileName());
    return FileName::fromString(settingsLocation.absolutePath() + path);
}


namespace LmBase {
namespace Internal {

Settings *Settings::m_instance = nullptr;

Settings::Settings()
    : QObject(nullptr)
{
    Q_ASSERT_X(!m_instance, Q_FUNC_INFO, "There can be only one Settings instance");
    m_instance = this;

    //set default values
    setChrootSettings(TargetSettings());
    setDeviceConnectivity(DeviceConnectivity());
    setProjectDefaults(ProjectDefaults());
    setDeviceAutoToggle(DEFAULT_DEVICES_AUTOTOGGLE);

    //create lm-sdk directory if it does not exist
    QString confdir = settingsPath().toString();
    QDir d = QDir::root();
    if(!d.exists(confdir)) {
        if(!d.mkpath(confdir))
            qWarning()<<"Unable to create Link Motion SDK configuration directory "<<confdir;
    }

    //tell the scripts where to find the configs
    qputenv("LMSDK_CONF_DIR",qPrintable(settingsPath().toString()));
}

Settings::~Settings()
{
    if (m_writer)
        delete m_writer;
}

FileName Settings::settingsPath()
{
    return FileName::fromString(
                settingsFileName(QLatin1String(LMSDK_FILENAME))
                .toFileInfo()
                .absolutePath());
}

void Settings::restoreSettings()
{
    QTC_ASSERT(!m_writer, return);
    m_writer = new PersistentSettingsWriter(
                settingsFileName(QLatin1String(LMSDK_FILENAME)),
                QLatin1String("LinkMotionSDKSettings"));

    PersistentSettingsReader read;
    if (read.load(settingsFileName(QLatin1String(LMSDK_FILENAME), QSettings::SystemScope)))
        m_settings = read.restoreValues();

    //load from user scope override system settings
    if (read.load(settingsFileName(QLatin1String(LMSDK_FILENAME)))) {
        QVariantMap userSettings = read.restoreValues();
        foreach (const QString &key, userSettings.keys())
            m_settings[key] = userSettings[key];
    }

    connect(Core::ICore::instance(), &Core::ICore::saveSettingsRequested,
            this, &Settings::flushSettings);
}

void Settings::flushSettings()
{
    m_instance->m_settings[QLatin1String(LMSDK_FILE_VERSION_KEY)] = 1;
    m_instance->m_writer->save(m_instance->m_settings, Core::ICore::mainWindow());
}

Settings::DeviceConnectivity Settings::deviceConnectivity()
{
    DeviceConnectivity val;
    val.ip = m_instance->m_settings.value(QLatin1String(KEY_IP),val.ip).toString();
    val.user = m_instance->m_settings.value(QLatin1String(KEY_USERNAME),val.user).toString();
    val.sshPort = m_instance->m_settings.value(QLatin1String(KEY_SSH),val.sshPort).toInt();
    return val;
}

void Settings::setDeviceConnectivity(const Settings::DeviceConnectivity &settings)
{
    m_instance->m_settings[QLatin1String(KEY_IP)] = settings.ip;
    m_instance->m_settings[QLatin1String(KEY_USERNAME)] = settings.user;
    m_instance->m_settings[QLatin1String(KEY_SSH)] = settings.sshPort;
}

Settings::ProjectDefaults Settings::projectDefaults()
{
    ProjectDefaults defaults;
    defaults.enableDebugHelper = m_instance->m_settings.value(QLatin1String(KEY_ENABLE_DEBUG_HELPER_DEFAULT),
                                                  defaults.enableDebugHelper).toBool();
    defaults.overrideAppsByDefault = m_instance->m_settings.value(QLatin1String(KEY_OVERRIDE_APPS_BY_DEFAULT),
                                                  defaults.overrideAppsByDefault).toBool();
    defaults.reviewErrorsAsWarnings = m_instance->m_settings.value(QLatin1String(KEY_TREAT_REVIEW_ERRORS_AS_WARNINGS),
                                                  defaults.reviewErrorsAsWarnings).toBool();
    defaults.uninstallAppsByDefault = m_instance->m_settings.value(QLatin1String(KEY_UNINSTALL_APPS_FROM_DEVICE_DEFAULT),
                                                  defaults.uninstallAppsByDefault).toBool();
    return defaults;
}

void Settings::setProjectDefaults(const Settings::ProjectDefaults &settings)
{
    m_instance->m_settings[QLatin1String(KEY_ENABLE_DEBUG_HELPER_DEFAULT)] = settings.enableDebugHelper;
    m_instance->m_settings[QLatin1String(KEY_OVERRIDE_APPS_BY_DEFAULT)] = settings.overrideAppsByDefault;
    m_instance->m_settings[QLatin1String(KEY_TREAT_REVIEW_ERRORS_AS_WARNINGS)] = settings.reviewErrorsAsWarnings;
    m_instance->m_settings[QLatin1String(KEY_UNINSTALL_APPS_FROM_DEVICE_DEFAULT)] = settings.uninstallAppsByDefault;
}

Settings::TargetSettings Settings::chrootSettings()
{
    TargetSettings val;
    val.useLocalMirror = m_instance->m_settings.value(QLatin1String(KEY_CHROOT_USE_LOCAL_MIRROR),val.useLocalMirror).toBool();
    return val;
}

void Settings::setChrootSettings(const Settings::TargetSettings &settings)
{
    m_instance->m_settings[QLatin1String(KEY_CHROOT_USE_LOCAL_MIRROR)]    = settings.useLocalMirror;
}

bool Settings::deviceAutoToggle()
{
    return m_instance->m_settings.value(QLatin1String(KEY_AUTOTOGGLE),
                            DEFAULT_DEVICES_AUTOTOGGLE).toBool();
}

void Settings::setDeviceAutoToggle(const bool set)
{
    m_instance->m_settings[QLatin1String(KEY_AUTOTOGGLE)] = set;
}

bool Settings::askForContainerSetup()
{
    return m_instance->m_settings.value(QLatin1String(KEY_ASK_FOR_CONTAINER_SETUP),
                            DEFAULT_ASK_FOR_CONTAINER_SETUP).toBool();
}

void Settings::setAskForContainerSetup(const bool set)
{
    m_instance->m_settings[QLatin1String(KEY_ASK_FOR_CONTAINER_SETUP)] = set;
}

} // namespace Internal
} // namespace LmBase
