#include "valuecache.h"

ValueCache::ValueCache(QObject *parent) : QObject(parent)
{

}

void ValueCache::processTempMeasurement(int16_t id, double tempC) {
    if(tempCache.contains(id)) {
        CacheItem_t prevVal = tempCache[id];
        const bool tempNew = tempC != prevVal.previousValue;
        const bool changeReasonable = abs(tempC - prevVal.previousValue) < 20.0;
        const bool rejectedMode = tempCache[id].rejectedMeasurements > 5;
        const bool update = tempNew && (changeReasonable || rejectedMode);
        const bool rejectMeasurement = !update;
        if(update) {
            tempCache[id].rejectedMeasurements = 0;
            tempCache[id].previousValue = tempC;
            emit this->newTempMeasurement(id, tempC);
        }
        if(rejectMeasurement) {
            tempCache[id].rejectedMeasurements = tempCache[id].rejectedMeasurements + 1;
        }
    } else {
        tempCache[id].previousValue = 0.0;
        tempCache[id].rejectedMeasurements = 0;
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
