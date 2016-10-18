#ifndef LINKMOTIONWELCOMEMODELS_H
#define LINKMOTIONWELCOMEMODELS_H

#include <QAbstractListModel>

namespace LinkMotion {
namespace Internal {

/* a copy from projectexplorer plugin */
/* under commercial license */
class SessionModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum { DefaultSessionRole = Qt::UserRole+1, LastSessionRole, ActiveSessionRole, ProjectsPathRole, ProjectsDisplayRole };

    explicit SessionModel(QObject *parent = 0);
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QHash<int, QByteArray> roleNames() const;

    Q_SCRIPTABLE bool isDefaultVirgin() const;

public slots:
    void resetSessions();
    void deleteSession(const QString &session);
};


/* a copy from projectexplorer plugin */
/* under commercial license */
class ProjectModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum { FilePathRole = Qt::UserRole+1, PrettyFilePathRole };

    ProjectModel(QObject *parent = 0);
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QHash<int, QByteArray> roleNames() const;

public slots:
    void resetProjects();
};

}
}


#endif // LINKMOTIONWELCOMEMODELS_H
