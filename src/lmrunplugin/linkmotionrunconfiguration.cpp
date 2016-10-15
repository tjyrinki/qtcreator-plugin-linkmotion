#include "linkmotionrunconfiguration.h"

#include <QDebug>
#include <qtsupport/qtoutputformatter.h>
#include <projectexplorer/target.h>

using namespace LinkMotion;
using namespace LinkMotion::Internal;

LinkMotionRunConfiguration::LinkMotionRunConfiguration(ProjectExplorer::Target *parent, Core::Id id)
    : RunConfiguration(parent, id)
{
    qDebug() << Q_FUNC_INFO;
}

LinkMotionRunConfiguration::LinkMotionRunConfiguration(ProjectExplorer::Target *parent, LinkMotionRunConfiguration *source)
    : RunConfiguration(parent, source)
{
    qDebug() << Q_FUNC_INFO;
}

QWidget *LinkMotionRunConfiguration::createConfigurationWidget()
{
    qDebug() << Q_FUNC_INFO;
    return 0;// no special running configurations
}

Utils::OutputFormatter *LinkMotionRunConfiguration::createOutputFormatter() const
{
    qDebug() << Q_FUNC_INFO;
    return new QtSupport::QtOutputFormatter(target()->project());
}
