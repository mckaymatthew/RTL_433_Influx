#include "udpsource.h"

UdpSource::UdpSource(uint32_t portListen, QObject *parent)
    : QObject(parent)
    , socket(this)
{
    auto bindResult = socket.bind(QHostAddress::Any, portListen);
    if(!bindResult) {
        qCritical() << "Failed to bind to port.";
    } else {
        connect(&socket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));
    }

}
UdpSource::~UdpSource() {
}
void UdpSource::processPendingDatagrams() {
    QHostAddress sender;
    unsigned short port;
    while (socket.hasPendingDatagrams())
    {
         QByteArray datagram;
         auto size = socket.pendingDatagramSize();
         datagram.resize(size);
         //qDebug() << QString("Got packet of size %1").arg(size);
         socket.readDatagram(datagram.data(),datagram.size(),&sender,&port);
         emit this->newMeasurement(datagram);
    }
}
