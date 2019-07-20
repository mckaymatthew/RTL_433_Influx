#ifndef VALUECACHE_H
#define VALUECACHE_H

#include <QObject>
#include <QMap>
#include <QElapsedTimer>

class ValueCache : public QObject
{
    Q_OBJECT
public:
    explicit ValueCache(QObject *parent = nullptr);

    struct {
        double previousValue;
        unsigned int rejectedMeasurements;
        QElapsedTimer timer;

    } typedef CacheItem_t;
    typedef QMap<int16_t, CacheItem_t> Cache_t;

signals:
    void newTempMeasurement(int16_t id, double tempC);
    void newHumidityMeasurement(int16_t id, double relH);
    void newRainRate(int16_t id, double rainRate);
    void newRainTotal(int16_t id, double rainTotal);
public slots:
    void processTempMeasurement(int16_t id, double tempC);
    void processHumidityMeasurement(int16_t id, double relH);
    void processRainRate(int16_t id, double rainRate);
    void processRainTotal(int16_t id, double rainTotal);
private:
    Cache_t tempCache;
    Cache_t humidCache;
    Cache_t rain;
    Cache_t rainTotal;
};

#endif // VALUECACHE_H
