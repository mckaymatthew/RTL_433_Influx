#include "influxposter.h"
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrlQuery>

InfluxPoster::InfluxPoster(QObject *parent)
    : QObject(parent)
    , manager(this)
    , m_settings("map.ini", QSettings::IniFormat)
{
    m_settings.setValue("Hoooo","");
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
}


void InfluxPoster::processTempMeasurement(int16_t id, double tempC) {
    QString lut = QString("Sensor%1").arg(id);
    if(m_settings.contains(lut)) {
        QString idStr = m_settings.value(lut).toString();
        QString postData = QString("temp,room=%1 value=%2").arg(idStr).arg(tempC);

        QNetworkRequest request(QUrl("http://10.0.0.23:8086/write?db=temp"));
        request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
        manager.post(request, postData.toUtf8());

    }

}
void InfluxPoster::processHumidityMeasurement(int16_t id, double relH) {
    QString lut = QString("Sensor%1").arg(id);
    if(m_settings.contains(lut)) {
        QString idStr = m_settings.value(lut).toString();
        QString postData = QString("humidity,room=%1 value=%2").arg(idStr).arg(relH);

        QNetworkRequest request(QUrl("http://10.0.0.23:8086/write?db=humid"));
        request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
        manager.post(request, postData.toUtf8());

    }

}