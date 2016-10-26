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


using namespace LinkMotion;
using namespace LinkMotion::Internal;

LinkMotionRunControl::LinkMotionRunControl(LinkMotionRunConfiguration *rc)
    : RunControl(rc, ProjectExplorer::Constants::NORMAL_RUN_MODE)
    , m_running(false)
{
    qDebug() << Q_FUNC_INFO;
   // setIcon(QLatin1String(ProjectExplorer::Constants::ICON_RUN_SMALL));
    connect(&m_process,SIGNAL(readyReadStandardError()),this,SLOT(onStdErr()));
    connect(&m_process,SIGNAL(readyReadStandardOutput()),this,SLOT(onStdOut()));
    connect(&m_process,SIGNAL(errorOccurred(QProcess::ProcessError)),this,SLOT(onError(QProcess::ProcessError)));
    connect(&m_process,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(onFinished(int, QProcess::ExitStatus)));
}

LinkMotionRunControl::~LinkMotionRunControl()
{
    qDebug() << Q_FUNC_INFO;
    stop();
}

void LinkMotionRunControl::onFinished(int code, QProcess::ExitStatus status) {
    qDebug() << Q_FUNC_INFO << code << status;
    stop();
}

void LinkMotionRunControl::onStdErr() {
    qDebug() << Q_FUNC_INFO;
    appendMessage(QString::fromLatin1(m_process.readAllStandardError()), Utils::ErrorMessageFormat);
}

void LinkMotionRunControl::onStdOut() {
    qDebug() << Q_FUNC_INFO;
    appendMessage(QString::fromLatin1(m_process.readAllStandardOutput()), Utils::NormalMessageFormat);
}

void LinkMotionRunControl::onError(QProcess::ProcessError err) {
    qDebug() << Q_FUNC_INFO << err << m_process.errorString();
    appendMessage(m_process.errorString(), Utils::ErrorMessageFormat);
    stop();
}

void LinkMotionRunControl::start()
{
    qDebug() << Q_FUNC_INFO;
    m_running = true;
    QStringList args;
    QString projectName = this->runConfiguration()->target()->project()->displayName();
    args << projectName;
    m_process.setArguments(args);
    emit started();
    appendMessage(tr("Starting remote process.\n"), Utils::NormalMessageFormat);
    Utils::Environment env = Utils::Environment::systemEnvironment();
    env.prependOrSetPath("/opt/linkmotion/sdk/vm");
    env.prependOrSetPath("/opt/linkmotion/sdk/hw");

    env.set(QStringLiteral("LINKMOTION_DEVICE"),QStringLiteral("intel"));
    env.set(QStringLiteral("LINKMOTION_USERNAME"),QStringLiteral("linkmotion"));
    env.set(QStringLiteral("LINKMOTION_PASSWORD"),QStringLiteral("notset"));

    m_process.setEnvironment(env.toStringList());
    qDebug() << m_process.environment() << args;

    //FIXME: env is not actually set!

    m_process.start(QStringLiteral("/opt/linkmotion/sdk/vm/vmsdk-app-start"),args);
    m_process.waitForStarted();

}

ProjectExplorer::RunControl::StopResult LinkMotionRunControl::stop()
{
    qDebug() << Q_FUNC_INFO;
    m_process.terminate();
    QString projectName = this->runConfiguration()->target()->project()->displayName();
    m_process.startDetached(QStringLiteral("/opt/linkmotion/sdk/vm/vmsdk-app-stop %0").arg(projectName));
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
