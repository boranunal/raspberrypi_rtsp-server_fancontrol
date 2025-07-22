#include "tcpclient.h"

TCPClient::TCPClient(QObject *parent) : QObject(parent) {
    connect(&socket, &QTcpSocket::readyRead, this, &TCPClient::onReadyRead);
    connect(&socket, &QTcpSocket::connected, this, &TCPClient::onConnected);
    connect(&m_updateTimer, &QTimer::timeout, this, &TCPClient::requestUpdate);
    m_updateTimer.start(4000);
    this->setPort(3000);
}

void TCPClient::connectToServer() {
    if (m_host.isEmpty() || m_port == 0) {
        qWarning() << "Host or port not set!";
        return;
    }
    if (socket.state() != QTcpSocket::ConnectedState){
        socket.connectToHost(m_host, m_port);
    }
}

void TCPClient::requestUpdate() {
    if (socket.state() == QAbstractSocket::ConnectedState) {
        QByteArray requestData = "GET";
        socket.write(requestData);
    }
    else {
        connectToServer();
    }
}

QString TCPClient::host() const {
    return m_host;
}

void TCPClient::setHost(const QString &host) {
    if (m_host == host)
        return;
    m_host = host;
    emit hostChanged();
}

quint16 TCPClient::port() const {
    return m_port;
}

void TCPClient::setPort(quint16 port) {
    if (m_port == port)
        return;
    m_port = port;
    emit portChanged();
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

void TCPClient::onConnected() {
    qDebug() << "Connected to server";
    requestUpdate();
}

float TCPClient::temperature() const {
    return m_temperature;
}

int TCPClient::dutyCycle() const {
    return m_dutyCycle;
}
