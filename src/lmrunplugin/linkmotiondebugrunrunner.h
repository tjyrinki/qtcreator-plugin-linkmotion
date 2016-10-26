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

    void slotProcess_Finished(int, QProcess::ExitStatus);
    void slotProcess_StateChanged(QProcess::ProcessState);
    void slotProcess_ReadyRead();
    void slotProcess_ReadyReadStandardError();
    void slotProcess_ReadyReadStandardOutput();

protected:
    ProjectExplorer::RunConfiguration* m_runConfig;
    Debugger::DebuggerRunControl* m_runControl;
    Utils::QtcProcess m_process;
};

}
}


#endif // LINKMOTIONDEBUGRUNRUNNER_H
