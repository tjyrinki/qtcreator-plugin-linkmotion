#ifndef LINKMOTIONRUNCONTROL_H
#define LINKMOTIONRUNCONTROL_H

#include <QObject>
#include <QProcess>
#include "linkmotionrunconfiguration.h"
#include <projectexplorer/runconfiguration.h>

namespace LinkMotion {
namespace Internal {

class LinkMotionRunControl : public ProjectExplorer::RunControl
{
    Q_OBJECT

public:
    explicit LinkMotionRunControl(LinkMotionRunConfiguration *runConfig);
    ~LinkMotionRunControl();

    void start();
    StopResult stop();
    bool isRunning() const;
    QString displayName() const;

protected slots:
    void emulateStop() { stop(); }

private:
    QProcess m_process;
    bool m_running;
};

}
}

#endif // LINKMOTIONRUNCONTROL_H
