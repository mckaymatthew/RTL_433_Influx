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
    void processRainRate(int16_t id, double rainRate);
    void processRainTotal(int16_t id, double rainTotal);
private:
    QNetworkAccessManager manager;
    QSettings m_settings;
    void write(int16_t id, double val, QString influxKey, QString db);
};


#endif // INFLUXPOSTER_H
