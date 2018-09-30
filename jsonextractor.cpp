#include "jsonextractor.h"
#include <QJsonDocument>
#include <QJsonObject>

JsonExtractor::JsonExtractor(QObject *parent) : QObject(parent)
{

}

void JsonExtractor::extract(QByteArray data) {
    auto doc = QJsonDocument::fromJson(data);
    if(!doc.isNull()) {
        const auto obj = doc.object();
        if(obj.contains("id")) {
            const auto idJson = obj.value("id");
            int16_t id = idJson.toInt();
            if(obj.contains("temperature_C")) {
                emit this->newTempMeasurement(id, obj.value("temperature_C").toDouble());
            }
            if(obj.contains("humidity")) {
                emit this->newHumidityMeasurement(id, obj.value("humidity").toDouble());
            }
        }
    }

}
