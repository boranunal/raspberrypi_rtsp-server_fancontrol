#include "tcpclient.h"

TCPClient::TCPClient(QObject *parent) : QObject(parent) {
    connect(&socket, &QTcpSocket::readyRead, this, &TCPClient::onReadyRead);
}

void TCPClient::connectToServer(const QString &host, quint16 port) {
    socket.connectToHost(host, port);
}

void TCPClient::requestUpdate() {
    socket.abort();
    socket.connectToHost("10.3.5.12", 3000); // Replace with your IP/port

    if (socket.waitForConnected(1000)) {
        socket.write("GET\n");
        socket.flush();
    }
}

void TCPClient::onReadyRead() {
    while (socket.canReadLine()) {
        QByteArray line = socket.readLine().trimmed();
        QList<QByteArray> parts = line.split(':');
        if (parts.size() == 2) {
            bool ok1, ok2;
            float temp = parts[0].toFloat(&ok1);
            int duty = parts[1].toInt(&ok2);
            if (ok1 && ok2) {
                if (qFuzzyCompare(m_temperature + 1, temp + 1) == false) {
                    m_temperature = temp;
                    emit temperatureChanged();
                }
                if (m_dutyCycle != duty) {
                    m_dutyCycle = duty;
                    emit dutyCycleChanged();
                }
            }
        }
    }
}

float TCPClient::temperature() const {
    return m_temperature;
}

int TCPClient::dutyCycle() const {
    return m_dutyCycle;
}
