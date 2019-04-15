#include "valuecache.h"
#include <cmath>

ValueCache::ValueCache(QObject *parent) : QObject(parent)
{

}

void ValueCache::processTempMeasurement(int16_t id, double tempC) {
    if(tempCache.contains(id)) {
        CacheItem_t prevVal = tempCache[id];
        const bool tempNew = tempC != prevVal.previousValue;
        const bool changeReasonable = std::abs(tempC - prevVal.previousValue) < 20.0;
        const bool rejectedMode = tempCache[id].rejectedMeasurements > 5;
        const bool update = tempNew && (changeReasonable || rejectedMode);
        const bool rejectMeasurement = !changeReasonable;
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
    if(humidCache.contains(id)) {
        CacheItem_t prevVal = humidCache[id];
        const bool humidNew = relH != prevVal.previousValue;
        const bool changeReasonable = std::abs(relH - prevVal.previousValue) < 10.0;
        const bool rejectedMode = humidCache[id].rejectedMeasurements > 5;
        const bool update = humidNew && (changeReasonable || rejectedMode);
        const bool rejectMeasurement = !changeReasonable;
        if(update) {
            humidCache[id].rejectedMeasurements = 0;
            humidCache[id].previousValue = relH;
            emit this->newHumidityMeasurement(id, relH);
        }
        if(rejectMeasurement) {
            humidCache[id].rejectedMeasurements = humidCache[id].rejectedMeasurements + 1;
        }
    } else {
        humidCache[id].previousValue = 0.0;
        humidCache[id].rejectedMeasurements = 0;
    }
}
