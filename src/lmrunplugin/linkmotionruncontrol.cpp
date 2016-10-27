/*####################################################################
#
# This file is part of the LinkMotion Run plugin.
#
# License: Proprietary
# Author: Juhapekka Piiroinen <juhapekka.piiroinen@link-motion.com>
#
# All rights reserved.
# (C) 2016 Link Motion Oy
####################################################################*/
#include "linkmotionruncontrol.h"

#include <QTimer>
#include <projectexplorer/project.h>
#include <projectexplorer/target.h>
#include <utils/environment.h>
#include <QDebug>
#include <projectexplorer/taskhub.h>
#include <projectexplorer/task.h>
#include "linkmotionrunplugin_constants.h"

using namespace LinkMotion;
using namespace LinkMotion::Internal;

LinkMotionRunControl::LinkMotionRunControl(LinkMotionRunConfiguration *rc)
    : RunControl(rc, ProjectExplorer::Constants::NORMAL_RUN_MODE)
    , m_running(false)
{
    qDebug() << Q_FUNC_INFO;
    connect(&m_processStart,SIGNAL(readyReadStandardError()),this,SLOT(slotStart_StdErr()));
    connect(&m_processStart,SIGNAL(readyReadStandardOutput()),this,SLOT(slotStart_StdOut()));
    connect(&m_processStart,SIGNAL(errorOccurred(QProcess::ProcessError)),this,SLOT(slotStart_Error(QProcess::ProcessError)));
    connect(&m_processStart,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(slotStart_Finished(int, QProcess::ExitStatus)));

    connect(&m_processStop,SIGNAL(readyReadStandardError()),this,SLOT(slotStop_StdErr()));
    connect(&m_processStop,SIGNAL(readyReadStandardOutput()),this,SLOT(slotStop_StdOut()));
    connect(&m_processStop,SIGNAL(errorOccurred(QProcess::ProcessError)),this,SLOT(slotStop_Error(QProcess::ProcessError)));
    connect(&m_processStop,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(slotStop_Finished(int, QProcess::ExitStatus)));
}

LinkMotionRunControl::~LinkMotionRunControl()
{
    qDebug() << Q_FUNC_INFO;
    stop();
}

void LinkMotionRunControl::slotStart_Finished(int code, QProcess::ExitStatus status) {
    qDebug() << Q_FUNC_INFO << code << status;
    stop();
}

void LinkMotionRunControl::slotStart_StdErr() {
    QByteArray data = m_processStart.readAllStandardError();
    qDebug() << Q_FUNC_INFO << data;

    QStringList lines = QString::fromLatin1(data).split("\n");

    foreach (QString line, lines) {
        Utils::OutputFormat outputFormat = Utils::StdOutFormat;
        if (line.startsWith(QStringLiteral("QQmlApplicationEngine failed to load component"))) {
            outputFormat = Utils::ErrorMessageFormat;
            ProjectExplorer::TaskHub::addTask(ProjectExplorer::Task::Error, line, LinkMotion::Constants::TASK_CATEGORY_RUN);
        } else if (line.startsWith(QStringLiteral("qrc:/"))) {
            outputFormat = Utils::NormalMessageFormat;
            ProjectExplorer::TaskHub::addTask(ProjectExplorer::Task::Error, line, LinkMotion::Constants::TASK_CATEGORY_RUN);
        }
        appendMessage(line, outputFormat);
        appendMessage(QString::fromLatin1("\n"),outputFormat);
    }
}

void LinkMotionRunControl::slotStart_StdOut() {
    QByteArray data = m_processStart.readAllStandardOutput();
    qDebug() << Q_FUNC_INFO << data;

    QStringList lines = QString::fromLatin1(data).split("\n");

    foreach (QString line, lines) {
        Utils::OutputFormat outputFormat = Utils::StdOutFormat;
        if (line.startsWith(QStringLiteral("Can't bind address"))) {
            ProjectExplorer::TaskHub::addTask(ProjectExplorer::Task::Error, line, LinkMotion::Constants::TASK_CATEGORY_RUN);
            outputFormat = Utils::ErrorMessageFormat;
        }
        appendMessage(line, outputFormat);
        appendMessage(QString::fromLatin1("\n"),outputFormat);
    }
}

void LinkMotionRunControl::slotStart_Error(QProcess::ProcessError err) {
    qDebug() << Q_FUNC_INFO << err << m_processStart.errorString();
    appendMessage(m_processStart.errorString(), Utils::ErrorMessageFormat);
    stop();
}

void LinkMotionRunControl::start()
{
    qDebug() << Q_FUNC_INFO;
    m_running = true;
    QString projectName = this->runConfiguration()->target()->project()->displayName();

    appendMessage(tr("Starting remote process.\n"), Utils::NormalMessageFormat);
    Utils::Environment env = Utils::Environment::systemEnvironment();
    env.prependOrSetPath("/opt/linkmotion/sdk/vm");
    env.prependOrSetPath("/opt/linkmotion/sdk/hw");

    //FIXME
    env.set(QStringLiteral("LINKMOTION_DEVICE"),QStringLiteral("intel"));
    env.set(QStringLiteral("LINKMOTION_USERNAME"),QStringLiteral("linkmotion"));
    env.set(QStringLiteral("LINKMOTION_PASSWORD"),QStringLiteral("notset"));

    m_processStart.setEnvironment(env);

    ProjectExplorer::TaskHub::clearTasks(LinkMotion::Constants::TASK_CATEGORY_RUN);

    m_processStart.setCommand(QStringLiteral("/opt/linkmotion/sdk/vm/vmsdk-app-start"),projectName);
    m_processStart.start();
    emit started();
}


void LinkMotionRunControl::slotStop_StdErr() {
    QByteArray data = m_processStop.readAllStandardError();
    qDebug() << Q_FUNC_INFO << data;
}

void LinkMotionRunControl::slotStop_StdOut() {
    QByteArray data = m_processStop.readAllStandardOutput();
    qDebug() << Q_FUNC_INFO << data;
}

void LinkMotionRunControl::slotStop_Error(QProcess::ProcessError err) {
    qDebug() << Q_FUNC_INFO << err << m_processStop.errorString();
    appendMessage(m_processStop.errorString(), Utils::ErrorMessageFormat);
}

void LinkMotionRunControl::slotStop_Finished(int code, QProcess::ExitStatus status) {
    qDebug() << Q_FUNC_INFO << code << status;
}

ProjectExplorer::RunControl::StopResult LinkMotionRunControl::stop()
{
    qDebug() << Q_FUNC_INFO;
    m_processStart.terminate();

    QString projectName = this->runConfiguration()->target()->project()->displayName();
    Utils::Environment env = Utils::Environment::systemEnvironment();
    env.prependOrSetPath("/opt/linkmotion/sdk/vm");
    env.prependOrSetPath("/opt/linkmotion/sdk/hw");

    //FIXME
    env.set(QStringLiteral("LINKMOTION_DEVICE"),QStringLiteral("intel"));
    env.set(QStringLiteral("LINKMOTION_USERNAME"),QStringLiteral("linkmotion"));
    env.set(QStringLiteral("LINKMOTION_PASSWORD"),QStringLiteral("notset"));

    m_processStop.setEnvironment(env);

    m_processStop.setCommand(QStringLiteral("/opt/linkmotion/sdk/vm/vmsdk-app-stop"),projectName);
    m_processStop.start();

    m_running = false;
    emit finished();
    appendMessage(tr("Stopped remote process."), Utils::NormalMessageFormat);
    return StoppedSynchronously;
}

bool LinkMotionRunControl::isRunning() const
{
    qDebug() << Q_FUNC_INFO;
    return m_running;
}

QString LinkMotionRunControl::displayName() const
{
    qDebug() << Q_FUNC_INFO;
    return QLatin1String("LinkMotion Runner");
}
