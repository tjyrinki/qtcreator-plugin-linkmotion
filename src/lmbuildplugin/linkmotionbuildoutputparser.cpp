#include "linkmotionbuildoutputparser.h"
#include <projectexplorer/task.h>
#include <projectexplorer/projectexplorerconstants.h>

#include <QRegularExpressionMatch>
#include <QDir>

LinkMotionBuildOutputParser::LinkMotionBuildOutputParser(const QString &projectName, const QString &arch) : ProjectExplorer::IOutputParser(),
    m_projectName(projectName),
    m_arch(arch),
    m_regexpPackage(QLatin1String("^Wrote: /root/rpmbuild/RPMS/(arm|intel)/(.*?)$")),
    m_regexpQrcWarning(QLatin1String("^(.*?qrc): Warning: (.*?)$")),
    m_regexpQMakeWarning(QLatin1String("^Project WARNING: (.*?)$")),
    m_regexpQMakeError(QLatin1String("^Project ERROR: (.*?)$")),
    m_regexpQMakeMessage(QLatin1String("^Project MESSAGE: (.*?)$")),
    m_regexpCompileWarning(QLatin1String("^([a-zA-Z_0-9\.]*?): (.*?)$")),
    m_regexpCompileWarningWithLine(QLatin1String("^([a-zA-Z_0-9\.]*?):([0-9]*):([0-9]*): (.*?)$"))
{
    qDebug() << Q_FUNC_INFO;

}

void LinkMotionBuildOutputParser::stdOutput(const QString &line) {
    qDebug() << Q_FUNC_INFO;
    QRegularExpressionMatch match = m_regexpPackage.match(line);

    if (match.hasMatch()) {
        Utils::FileName package = Utils::FileName::fromUserInput(match.captured(1));
        Utils::FileName packageFullLocation = Utils::FileName::fromUserInput(QDir(QString(QLatin1String("%0/build-%1-%2-latest/%3")).arg(m_workingDirectory).arg(m_projectName).arg(m_arch).arg(package.toString())).absolutePath());
        taskAdded(ProjectExplorer::Task(ProjectExplorer::Task::Unknown, QLatin1String("Package was built."),
                       packageFullLocation /* filename */,
                       -1, /* line */
                       Core::Id(ProjectExplorer::Constants::TASK_CATEGORY_BUILDSYSTEM)), 1, 0);
    }
}

void LinkMotionBuildOutputParser::stdError(const QString &line) {
    qDebug() << Q_FUNC_INFO;
    QRegularExpressionMatch match = m_regexpQMakeWarning.match(line);
    if (match.hasMatch()) {
        taskAdded(ProjectExplorer::Task(ProjectExplorer::Task::Warning, line,
                       Utils::FileName::fromLatin1("") /* filename */,
                       -1, /* line */
                       Core::Id(ProjectExplorer::Constants::TASK_CATEGORY_BUILDSYSTEM)), 1, 0);
    }
    match = m_regexpQMakeError.match(line);
    if (match.hasMatch()) {
        taskAdded(ProjectExplorer::Task(ProjectExplorer::Task::Error, line,
                       Utils::FileName::fromLatin1("") /* filename */,
                       -1, /* line */
                       Core::Id(ProjectExplorer::Constants::TASK_CATEGORY_BUILDSYSTEM)), 1, 0);
    }
    match = m_regexpQMakeMessage.match(line);
    if (match.hasMatch()) {
        taskAdded(ProjectExplorer::Task(ProjectExplorer::Task::Unknown, line,
                       Utils::FileName::fromLatin1("") /* filename */,
                       -1, /* line */
                       Core::Id(ProjectExplorer::Constants::TASK_CATEGORY_BUILDSYSTEM)), 1, 0);
    }

    match = m_regexpCompileWarningWithLine.match(line);
    if (match.hasMatch()) {
        if (!match.captured(0).startsWith(QLatin1String("error:"))) {
            if (!match.captured(0).startsWith(QLatin1String("warning:"))) {
                if (!match.captured(0).startsWith(QLatin1String("WARNING:"))) {
                    if (!match.captured(0).startsWith(QLatin1String("scp:"))) {
                        taskAdded(ProjectExplorer::Task(ProjectExplorer::Task::Warning, line,
                                       Utils::FileName::fromString(match.captured(1)) /* filename */,
                                       match.captured(2).toInt(), /* line */
                                       Core::Id(ProjectExplorer::Constants::TASK_CATEGORY_BUILDSYSTEM)), 1, 0);
                    }
                }
            }
        }
    } else {
        match = m_regexpCompileWarning.match(line);
        if (match.hasMatch()) {
            if (!match.captured(0).startsWith(QLatin1String("error:"))) {
                if (!match.captured(0).startsWith(QLatin1String("warning:"))) {
                    if (!match.captured(0).startsWith(QLatin1String("WARNING:"))) {
                        if (!match.captured(0).startsWith(QLatin1String("scp:"))) {
                            taskAdded(ProjectExplorer::Task(ProjectExplorer::Task::Warning, line,
                                           Utils::FileName::fromString(match.captured(1)) /* filename */,
                                           -1, /* line */
                                           Core::Id(ProjectExplorer::Constants::TASK_CATEGORY_BUILDSYSTEM)), 1, 0);
                        }
                    }
                }
            }
        }
    }

    match = m_regexpQrcWarning.match(line);
    if (match.hasMatch()) {
        taskAdded(ProjectExplorer::Task(ProjectExplorer::Task::Warning, line,
                       Utils::FileName::fromString(match.captured(1)) /* filename */,
                       -1, /* line */
                       Core::Id(ProjectExplorer::Constants::TASK_CATEGORY_BUILDSYSTEM)), 1, 0);

    }
    /*
     * ../../../src/qml/qml.qrc: Warning: potential duplicate alias detected: 'enjoy_icon_radio_scan.svg'
     *
     *
     * ListMusicModel_1_0.cpp: In member function 'virtual QHash<int, QByteArray> ListMusicModel_1_0::roleNames() const':
ListMusicModel_1_0.cpp:4:61: warning: no return statement in function returning non-void [-Wreturn-type]
 QHash<int,QByteArray> ListMusicModel_1_0::roleNames() const{}
     *
     * Project WARNING: QML Compiler is enabled.
     *
     * error: package lm-ui-plugins is not installed
     *
     * Module 'QtSparql' does not contain a module identifier directive - it cannot be protected from external registrations.
     *
     *
     * In file included from bluetoothprofilesession.cpp:21:0:
bluetoothprofilesession.h: In constructor 'BluetoothProfileSession::BluetoothProfileSession(BluetoothManager*, QObject*)':
bluetoothprofilesession.h:57:21: warning: 'BluetoothProfileSession::m_sessionInterface' will be initialized after [-Wreorder]
     QDBusInterface *m_sessionInterface;
                     ^
bluetoothprofilesession.h:53:10: warning:   'bool BluetoothProfileSession::m_creatingSession' [-Wreorder]
     bool m_creatingSession;
          ^
bluetoothprofilesession.cpp:25:1: warning:   when initialized here [-Wreorder]
 BluetoothProfileSession::BluetoothProfileSession(BluetoothManager *btm, QObject *parent) :
 ^
     *

     * warning: File listed twice: /usr/lib/debug/.build-id
     *
     */
}
