#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);
    app.setApplicationName(QString::fromLatin1("%ProjectName%"));
    app.setApplicationVersion(QString::fromLatin1("1.0"));
    app.setApplicationDisplayName(QString::fromLatin1("%ProjectName%"));
    app.setOrganizationDomain(QString::fromLatin1("com.example"));
    app.setOrganizationName(QString::fromLatin1("Example"));

    QQmlApplicationEngine engine;
    engine.addImportPath(QString("%0/../modules").arg(app.applicationDirPath()));

#ifdef QT_DEBUG
    qDebug() << "Import Path" << engine.importPathList();
    qDebug() << "Application Name" << app.applicationName();
    qDebug() << "Application Version" << app.applicationVersion();
    qDebug() << "Organization Domain" << app.organizationDomain();
    qDebug() << "Organization Name" << app.organizationName();
#endif

    engine.load(QUrl(QLatin1String("qrc:/main.qml")));

    return app.exec();
}
