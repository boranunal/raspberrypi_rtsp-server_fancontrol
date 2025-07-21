#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>

class TCPClient : public QObject {
    Q_OBJECT
    Q_PROPERTY(float temperature READ temperature NOTIFY temperatureChanged)
    Q_PROPERTY(int dutyCycle READ dutyCycle NOTIFY dutyCycleChanged)
public:
    explicit TCPClient(QObject *parent = nullptr);
    Q_INVOKABLE void requestUpdate();

    void connectToServer(const QString &host, quint16 port);

    float temperature() const;
    int dutyCycle() const;

signals:
    void temperatureChanged();
    void dutyCycleChanged();

private slots:
    void onReadyRead();

private:
    QTcpSocket socket;
    float m_temperature = 0.0;
    int m_dutyCycle = 0;
};

#endif // TCPCLIENT_H
