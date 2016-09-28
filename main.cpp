#include <QGuiApplication>
#include <QtQml>
#include <QQmlApplicationEngine>
#include "mediaclass.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    qmlRegisterType<MediaClass>("com.liuyang.MediaType", 1, 0, "MediaType"); //Register

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}

