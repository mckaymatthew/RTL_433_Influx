#include "valuecache.h"

ValueCache::ValueCache(QObject *parent) : QObject(parent)
{

}

void ValueCache::processTempMeasurement(int16_t id, double tempC) {
    //Rely on auto initialization of the key;
    double prevVal = tempCache[id];
    if(tempC != prevVal) {
        tempCache[id] = tempC;
        emit this->newTempMeasurement(id, tempC);
    }

}
void ValueCache::processHumidityMeasurement(int16_t id, double relH) {
    //Rely on auto initialization of the key;
    double prevVal = humidCache[id];
    if(relH != prevVal) {
        humidCache[id] = relH;
        emit this->newHumidityMeasurement(id, relH);
    }
}
