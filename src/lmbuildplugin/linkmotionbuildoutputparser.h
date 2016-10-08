#ifndef LINKMOTIONBUILDOUTPUTPARSER_H
#define LINKMOTIONBUILDOUTPUTPARSER_H

#include <QObject>
#include <projectexplorer/ioutputparser.h>
#include <QRegularExpression>

class LinkMotionBuildOutputParser : public ProjectExplorer::IOutputParser
{
    Q_OBJECT

public:
    LinkMotionBuildOutputParser(const QString &projectName, const QString &arch);

    void stdOutput(const QString &line);
    void stdError(const QString &line);

    void setWorkingDirectory(const QString &workingDirectory) {
        m_workingDirectory = workingDirectory;
        ProjectExplorer::IOutputParser::setWorkingDirectory(workingDirectory);
    }

    void setProjectName(const QString &projectName) {
        m_projectName = projectName;
    }

protected:
    QString m_workingDirectory;
    QString m_projectName;
    QString m_arch;
    QRegularExpression m_regexpQrcWarning;
    QRegularExpression m_regexpPackage;
    QRegularExpression m_regexpQMakeWarning;
    QRegularExpression m_regexpQMakeError;
    QRegularExpression m_regexpQMakeMessage;
    QRegularExpression m_regexpCompileWarning;
    QRegularExpression m_regexpCompileWarningWithLine;
};

#endif // LINKMOTIONBUILDOUTPUTPARSER_H
