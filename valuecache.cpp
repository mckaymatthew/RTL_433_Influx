#include "valuecache.h"
#include <cmath>

namespace {
bool evaluateUpdate(ValueCache::Cache_t & cache, int16_t id, double newValue, double maxReasonableChange) {
    bool doEmit = false;

    if(cache.contains(id)) {
        ValueCache::CacheItem_t prevVal = cache[id];
        const bool tempNew = newValue != prevVal.previousValue;
        const bool timedUpdate = cache[id].timer.elapsed() > (60 * 60 * 1000);

        const bool changeReasonable = std::abs(newValue - prevVal.previousValue) < maxReasonableChange;
        const bool rejectedMode = cache[id].rejectedMeasurements > (3 * 2 * 3); //each sensor is 3 tx, 2 recievers, 3 bad mesurements

        const bool update = (timedUpdate || tempNew) && (changeReasonable || rejectedMode);
        const bool rejectMeasurement = !changeReasonable;
        if(update) {
            cache[id].rejectedMeasurements = 0;
            cache[id].previousValue = newValue;
            cache[id].timer.start();
            doEmit = true;
        } else if(rejectMeasurement) {
            cache[id].rejectedMeasurements = cache[id].rejectedMeasurements + 1;
        }
    } else {
        cache[id].rejectedMeasurements = 0;
        cache[id].previousValue = newValue;
        cache[id].timer.start();
        doEmit = true;
    }

    return doEmit;
}
}

ValueCache::ValueCache(QObject *parent)
    : QObject(parent)
    , tempCache()
    , humidCache()
    , rain()
    , rainTotal()
{

}

void ValueCache::processTempMeasurement(int16_t id, double tempC) {
    const bool doEmit = evaluateUpdate(tempCache, id, tempC, 20);
    if(doEmit) {
        emit this->newTempMeasurement(id, tempC);
    }
}
void ValueCache::processHumidityMeasurement(int16_t id, double relH) {
    const bool doEmit = evaluateUpdate(humidCache, id, relH, 20);
    if(doEmit) {
        emit this->newHumidityMeasurement(id, relH);
    }
}
void ValueCache::processRainRate(int16_t id, double rainRate) {
    const bool doEmit = evaluateUpdate(rain, id, rainRate, 200); //todo: figure out reasonable mm:hour change between updates of sensor
    if(doEmit) {
        emit this->newRainRate(id, rainRate);
    }
}
void ValueCache::processRainTotal(int16_t id, double rainTotalp) {
    const bool doEmit = evaluateUpdate(rainTotal, id, rainTotalp, 200); //todo: same as above
    if(doEmit) {
        emit this->newRainTotal(id, rainTotalp);
    }
}
