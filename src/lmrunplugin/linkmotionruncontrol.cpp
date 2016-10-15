#include "linkmotionruncontrol.h"

#include <QTimer>
#include <projectexplorer/project.h>
#include <projectexplorer/target.h>
#include <QDebug>


using namespace LinkMotion;
using namespace LinkMotion::Internal;

LinkMotionRunControl::LinkMotionRunControl(LinkMotionRunConfiguration *rc)
    : RunControl(rc, ProjectExplorer::Constants::NORMAL_RUN_MODE)
    , m_running(false)
{
    qDebug() << Q_FUNC_INFO;
    setIcon(QLatin1String(ProjectExplorer::Constants::ICON_RUN_SMALL));
}

LinkMotionRunControl::~LinkMotionRunControl()
{
    qDebug() << Q_FUNC_INFO;
    stop();
}

void LinkMotionRunControl::start()
{
    qDebug() << Q_FUNC_INFO;
    m_running = true;
    //vmsdk-shell /altdata/apps/untitled117/bin/untitled117 -plugin vboxtouch -platform eglfs
    QStringList args;
    QString projectName = this->runConfiguration()->target()->project()->displayName();
    args << QStringLiteral("/altdata/apps/%0/bin/%0").arg(projectName);
    args << QStringLiteral("-plugin vboxtouch");
    args << QStringLiteral("-platform eglfs");
    m_process.setArguments(args);
    emit started();
    appendMessage(tr("Starting remote process."), Utils::NormalMessageFormat);
    m_process.start(QStringLiteral("vmsdk-shell"),args);
}

ProjectExplorer::RunControl::StopResult LinkMotionRunControl::stop()
{
    qDebug() << Q_FUNC_INFO;
    m_process.kill();
    // FIXME: does not kill the process on remote machine.
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
