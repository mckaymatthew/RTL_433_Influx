#include <QCoreApplication>
#include "udpsource.h"
#include "stdinsource.h"
#include "jsonextractor.h"
#include "valuecache.h"
#include "influxposter.h"
#include <initializer_list>
#include <signal.h>
#include <unistd.h>

void catchUnixSignals(std::initializer_list<int> quitSignals);
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QSettings settings;
    UdpSource src(40007);
    //StdInSource src;
    JsonExtractor extract;
    ValueCache cache;
    InfluxPoster poster;
    catchUnixSignals({SIGQUIT, SIGINT, SIGTERM, SIGHUP});

    auto ext = &JsonExtractor::extract;
    a.connect(&src, &UdpSource::newMeasurement, &extract, ext);
    //a.connect(&src, &StdInSource::newMeasurement, &extract, ext);

    a.connect(&extract, &JsonExtractor::newHumidityMeasurement, &cache, &ValueCache::processHumidityMeasurement);
    a.connect(&extract, &JsonExtractor::newTempMeasurement, &cache, &ValueCache::processTempMeasurement);
    a.connect(&extract, &JsonExtractor::newRainRate, &cache, &ValueCache::processRainRate);
    a.connect(&extract, &JsonExtractor::newRainTotal, &cache, &ValueCache::processRainTotal);

    a.connect(&cache, &ValueCache::newHumidityMeasurement, [](int16_t id, double val){ qWarning() << QString("Humid: %1: %2").arg(id).arg(val); });
    a.connect(&cache, &ValueCache::newTempMeasurement, [](int16_t id, double val){ qWarning() << QString("Temp: %1: %2").arg(id).arg(val); });
    a.connect(&cache, &ValueCache::newRainRate, [](int16_t id, double val){ qWarning() << QString("Rain Rate: %1: %2").arg(id).arg(val); });
    a.connect(&cache, &ValueCache::newRainTotal, [](int16_t id, double val){ qWarning() << QString("Rain Total: %1: %2").arg(id).arg(val); });

    a.connect(&cache, &ValueCache::newHumidityMeasurement, &poster, &InfluxPoster::processHumidityMeasurement);
    a.connect(&cache, &ValueCache::newTempMeasurement, &poster, &InfluxPoster::processTempMeasurement);
    a.connect(&cache, &ValueCache::newRainRate, &poster, &InfluxPoster::processRainRate);
    a.connect(&cache, &ValueCache::newRainTotal, &poster, &InfluxPoster::processRainTotal);


    qWarning() << "Started.";
    return a.exec();
}

void catchUnixSignals(std::initializer_list<int> quitSignals) {
    auto handler = [](int sig) -> void {
        QCoreApplication::quit();
    };

    sigset_t blocking_mask;
    sigemptyset(&blocking_mask);
    for (auto sig : quitSignals)
        sigaddset(&blocking_mask, sig);

    struct sigaction sa;
    sa.sa_handler = handler;
    sa.sa_mask    = blocking_mask;
    sa.sa_flags   = 0;

    for (auto sig : quitSignals)
        sigaction(sig, &sa, nullptr);
}
