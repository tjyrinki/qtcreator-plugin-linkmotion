#ifndef LINKMOTIONDEBUGRUNCONTROL_H
#define LINKMOTIONDEBUGRUNCONTROL_H

#include <QObject>
#include <QDebug>
#include "../lmrunplugin/linkmotionruncontrol.h"
#include <debugger/debuggerruncontrol.h>

namespace LinkMotion {
namespace Internal {

class LinkMotionDebugRunControl : public QObject
{
    Q_OBJECT
public:
    explicit LinkMotionDebugRunControl(ProjectExplorer::RunConfiguration *runConfig, Debugger::DebuggerRunControl *runControl);

    static ProjectExplorer::RunControl *create(ProjectExplorer::RunConfiguration *runConfig, QString *errorMessage);

protected slots:
    void onStdOut();
    void onStdErr();
    void onFinished(int, QProcess::ExitStatus);

    int getProcessPid();
    int getPort();
    int getQmlPort();
    void detectGdbServer();

    void start();


private:

    Debugger::DebuggerRunControl *m_runControl;
    QProcess m_process;
    QByteArray m_stdout;
    QString m_projectName;
    QString m_appName;
    int m_pid;
    int m_gdbPort;
    int m_qmlPort;
};

}
}

#endif // LINKMOTIONDEBUGRUNCONTROL_H
