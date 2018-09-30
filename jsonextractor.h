#ifndef JSONEXTRACTOR_H
#define JSONEXTRACTOR_H

#include <QObject>

class JsonExtractor : public QObject
{
    Q_OBJECT
public:
    explicit JsonExtractor(QObject *parent = nullptr);

signals:
    void newTempMeasurement(int16_t id, double tempC);
    void newHumidityMeasurement(int16_t id, double relH);
public slots:
    void extract(QByteArray data);
};

#endif // JSONEXTRACTOR_H
