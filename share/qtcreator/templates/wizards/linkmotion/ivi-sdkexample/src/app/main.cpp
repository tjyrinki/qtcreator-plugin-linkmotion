#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.addImportPath(QString("%0/../modules").arg(app.applicationDirPath()));

#ifdef QT_DEBUG
    qDebug() << "Import Path" << engine.importPathList();
#endif

    engine.load(QUrl(QLatin1String("qrc:/main.qml")));

    return app.exec();
}
