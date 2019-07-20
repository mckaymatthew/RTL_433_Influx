#include "influxposter.h"
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrlQuery>

InfluxPoster::InfluxPoster(QObject *parent)
    : QObject(parent)
    , manager(this)
    , m_settings("map.ini", QSettings::IniFormat)
{
    m_settings.sync();
    QObject::connect(&manager, &QNetworkAccessManager::finished,
        this, [=](QNetworkReply *reply) {
            if (reply->error()) {
                qDebug() << reply->errorString();
                return;
            }

            QString answer = reply->readAll();
            reply->deleteLater();

        }
    );
    if(!m_settings.contains("host")) {
        qWarning() << QString("\"host\" parameter is missing from map.ini, this will probably not work.");

    }
}

void InfluxPoster::write(int16_t id, double val, QString influxKey, QString db) {

    QString lut = QString("Sensor%1").arg(id);
    if(m_settings.contains(lut)) {
        QString idStr = m_settings.value(lut).toString();
        QString postData = QString("%3=%1 value=%2").arg(idStr).arg(val).arg(influxKey);
        QString postUrl = QString("http://%1:8086/write?db=%2").arg(m_settings.value("host").toString()).arg(db);
        QNetworkRequest request;
        request.setUrl(QUrl(postUrl));

        request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
        manager.post(request, postData.toUtf8());

    } else {
        qWarning() << QString("Sensor Not in LUT: %1").arg(id);
    }
}
void InfluxPoster::processRainRate(int16_t id, double rainRate) {
    write(id, rainRate, "rainRate,location","rain");
}
void InfluxPoster::processRainTotal(int16_t id, double rainTotal) {
    write(id, rainTotal, "rainTotal,location","rain");
}
void InfluxPoster::processTempMeasurement(int16_t id, double tempC) {
    write(id, tempC, "temp,room","temp");
}
void InfluxPoster::processHumidityMeasurement(int16_t id, double relH) {
    write(id, relH, "humidity,room","humid");
}
