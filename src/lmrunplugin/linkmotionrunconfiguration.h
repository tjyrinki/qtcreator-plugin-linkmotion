#ifndef LINKMOTIONRUNCONFIGURATION_H
#define LINKMOTIONRUNCONFIGURATION_H

#include <QObject>
#include <projectexplorer/runconfiguration.h>

namespace LinkMotion {
namespace Internal {

class LinkMotionRunConfiguration : public ProjectExplorer::RunConfiguration
{
    Q_OBJECT
public:
    LinkMotionRunConfiguration(ProjectExplorer::Target *parent, Core::Id id);
    LinkMotionRunConfiguration(ProjectExplorer::Target *parent, LinkMotionRunConfiguration *source);

    QWidget *createConfigurationWidget();
    Utils::OutputFormatter *createOutputFormatter() const;

};

}
}

#endif // LINKMOTIONRUNCONFIGURATION_H
