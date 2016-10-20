#ifndef LINKMOTIONANALYZERUNCONTROL_H
#define LINKMOTIONANALYZERUNCONTROL_H

#include <QObject>
#include "linkmotionruncontrol.h"

namespace Analyzer { class AnalyzerRunControl; }

namespace LinkMotion {
namespace Internal {

class LinkMotionAnalyzeRunControl : public QObject
{
    Q_OBJECT

public:
    LinkMotionAnalyzeRunControl(LinkMotionRunConfiguration *runConfig, Analyzer::AnalyzerRunControl *runControl);
    static ProjectExplorer::RunControl *create(LinkMotionRunConfiguration *runConfig, Core::Id runMode);

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

    Analyzer::AnalyzerRunControl *m_runControl;
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


#endif // LINKMOTIONANALYZERUNCONTROL_H
