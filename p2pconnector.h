#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

#include <QObject>
#include <QUdpSocket>
#include <QHostAddress>

class Tp2pConnector : public QUdpSocket
{
    Q_OBJECT

    static int  hostMsgSize;
    static char hostRequest[];
    static char hostAnswer[];

    struct TCommParams
    {
        QHostAddress ownHost;
        quint16      ownReadPort;
        QHostAddress remoteHost;
        quint16      remoteWritePort;
    }   params;

    qint8       handshaking;
    bool        ready;

    void swapPorts();
    void checkOrder(const char*, const int);

public:
    Tp2pConnector(QObject* = nullptr, quint16 = 10000);
    bool isReady();
    void handshake();

signals:
    void recieved(QByteArray);
    void handshaked(int);

public slots:
    void send(QByteArray);

private slots:
    void recieve();

};

#endif // COMMUNICATOR_H
