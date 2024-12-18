#include "p2pconnector.h"
#include <QHostInfo>

int  Tp2pConnector::hostMsgSize = 4;
char Tp2pConnector::hostRequest[] = {0x05,0x00,0x01,0x08};
char Tp2pConnector::hostAnswer[]  = {0x05,0x01,0x00,0x08};

Tp2pConnector::Tp2pConnector(QObject *parent, quint16 basePort) : QUdpSocket(parent)
{
    handshaking = -1;
    params.ownHost = QHostAddress::LocalHost;
    params.remoteHost = QHostAddress::LocalHost;
    params.ownReadPort = basePort;
    params.remoteWritePort = basePort + 1;
    ready = bind(params.ownHost, params.ownReadPort);
    if (!ready)
    {
        swapPorts();
        ready = bind(params.ownHost, params.ownReadPort);
    }
    if (ready) connect(this,SIGNAL(readyRead()),this,SLOT(recieve()));
}

void Tp2pConnector::handshake()
{
    if (handshaking < 0)
    {
        handshaking = 0;
        send(QByteArray(hostRequest,hostMsgSize));
    }
}

void Tp2pConnector::swapPorts()
{
    quint16 p = params.ownReadPort;
    params.ownReadPort = params.remoteWritePort;
    params.remoteWritePort = p;
}

void Tp2pConnector::checkOrder(const char* msg, const int size)
{
    if (size == hostMsgSize)
    {
        if (memcmp(msg,hostRequest,size) == 0)
        {
            send(QByteArray(hostAnswer,hostMsgSize));
            emit handshaked(1);
            handshaking = 1;
        }
        if (memcmp(msg,hostAnswer,size) == 0)
        {
            emit handshaked(2);
            handshaking = 1;
        }
    }
}

bool Tp2pConnector::isReady()
{
    return ready;
}

void Tp2pConnector::send(QByteArray msg)
{
    if (ready)
    {
        writeDatagram(msg, params.remoteHost, params.remoteWritePort);
    }
}

void Tp2pConnector::recieve()
{
    if (hasPendingDatagrams())
    {
        quint64 size = pendingDatagramSize();
        QByteArray msg(size,'\0');
        readDatagram(msg.data(), size);
        if (handshaking == 0)
            checkOrder(msg.data(),size);
        else
            emit recieved(msg);
    }
}
