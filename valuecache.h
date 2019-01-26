#ifndef VALUECACHE_H
#define VALUECACHE_H

#include <QObject>
#include <QMap>

class ValueCache : public QObject
{
    Q_OBJECT
public:
    explicit ValueCache(QObject *parent = nullptr);

signals:
    void newTempMeasurement(int16_t id, double tempC);
    void newHumidityMeasurement(int16_t id, double relH);
public slots:
    void processTempMeasurement(int16_t id, double tempC);
    void processHumidityMeasurement(int16_t id, double relH);
private:
    struct {
        double previousValue;
        unsigned int rejectedMeasurements;
    } typedef CacheItem_t
    typedef QMap<int16_t, CacheItem_t> Cache_t;
    Cache_t tempCache;
    Cache_t humidCache;
};

#endif // VALUECACHE_H
