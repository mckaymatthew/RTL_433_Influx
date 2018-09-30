#ifndef INFLUXPOSTER_H
#define INFLUXPOSTER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QSettings>

class InfluxPoster : public QObject
{
    Q_OBJECT
public:
    explicit InfluxPoster(QObject *parent = nullptr);

signals:

public slots:
    void processTempMeasurement(int16_t id, double tempC);
    void processHumidityMeasurement(int16_t id, double relH);
private:
    QNetworkAccessManager manager;
    QSettings m_settings;
};

#endif // INFLUXPOSTER_H
