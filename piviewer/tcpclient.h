#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QTimer>

class TCPClient : public QObject {
    Q_OBJECT
    Q_PROPERTY(float temperature READ temperature NOTIFY temperatureChanged)
    Q_PROPERTY(int dutyCycle READ dutyCycle NOTIFY dutyCycleChanged)
    Q_PROPERTY(QString host READ host WRITE setHost NOTIFY hostChanged)
    Q_PROPERTY(quint16 port READ port WRITE setPort NOTIFY portChanged)
public:
    explicit TCPClient(QObject *parent = nullptr);
    Q_INVOKABLE void requestUpdate();
    Q_INVOKABLE void connectToServer();

    float temperature() const;
    int dutyCycle() const;

    QString host() const;
    void setHost(const QString &host);

    quint16 port() const;
    void setPort(quint16 port);

signals:
    void temperatureChanged();
    void dutyCycleChanged();

    void hostChanged();
    void portChanged();

private slots:
    void onReadyRead();
    void onConnected();

private:
    QTcpSocket socket;
    float m_temperature = 0.0;
    int m_dutyCycle = 0;
    QString m_host;
    quint16 m_port;
    QTimer m_updateTimer;
};

#endif // TCPCLIENT_H
