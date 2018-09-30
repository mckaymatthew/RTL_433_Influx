#ifndef UDPSOURCE_H
#define UDPSOURCE_H

#include <QObject>
#include <QUdpSocket>

class UdpSource : public QObject
{
    Q_OBJECT
public:
    explicit UdpSource(uint32_t portListen = 40004, QObject *parent = nullptr);
    ~UdpSource();

signals:
    void newMeasurement(QByteArray json);
private slots:
    void processPendingDatagrams();
private:
    QUdpSocket socket;
};

#endif // UDPSOURCE_H
