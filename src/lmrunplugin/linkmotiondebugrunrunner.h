#ifndef LINKMOTIONDEBUGRUNRUNNER_H
#define LINKMOTIONDEBUGRUNRUNNER_H

#include <QObject>
#include <projectexplorer/runconfiguration.h>
#include <debugger/debuggerruncontrol.h>
#include <utils/qtcprocess.h>
#include <QProcess>

namespace LinkMotion {
namespace Internal {

class LinkMotionDebugRunRunner : public QObject
{
    Q_OBJECT

public:
    LinkMotionDebugRunRunner(ProjectExplorer::RunConfiguration *runConfig,
                             Debugger::DebuggerRunControl *runControl);

protected slots:
    void slotRunControl_RequestRemoteSetup();
    void slotRunControl_Finished();
    void slotRunControl_Started();
    void slotRunControl_StateChanged(Debugger::DebuggerState state);

    void slotStart_Finished(int, QProcess::ExitStatus);
    void slotStart_StateChanged(QProcess::ProcessState);
    void slotStart_ReadyRead();
    void slotStart_ReadyReadStandardError();
    void slotStart_ReadyReadStandardOutput();

    void slotStop_Finished(int, QProcess::ExitStatus);
    void slotStop_StateChanged(QProcess::ProcessState);
    void slotStop_ReadyRead();
    void slotStop_ReadyReadStandardError();
    void slotStop_ReadyReadStandardOutput();
protected:
    ProjectExplorer::RunConfiguration* m_runConfig;
    Debugger::DebuggerRunControl* m_runControl;
    Utils::QtcProcess m_processStart;
    Utils::QtcProcess m_processStop;
};

}
}


#endif // LINKMOTIONDEBUGRUNRUNNER_H
