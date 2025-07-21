#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "tcpclient.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    TCPClient client;
    client.connectToServer("10.3.5.12", 3000);

    engine.rootContext()->setContextProperty("tcpClient", &client);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
