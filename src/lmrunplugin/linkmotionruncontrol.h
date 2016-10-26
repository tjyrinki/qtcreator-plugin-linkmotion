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

    void onStdOut();
    void onStdErr();
    void onFinished(int, QProcess::ExitStatus);
    void onError(QProcess::ProcessError err);

private:
    QProcess m_process;
    bool m_running;
};

}
}

#endif // LINKMOTIONRUNCONTROL_H
