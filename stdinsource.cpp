#include "stdinsource.h"
#include <string>
#include <iostream>
#include <QUdpSocket>

StdInSource::StdInSource(QObject *parent) : QObject(parent)
{
    startTimer(250);
}

void StdInSource::timerEvent(QTimerEvent * ev) {
    qDebug() << "Foo";

    while (std::cin) {
        std::string line;
        std::getline(std::cin, line);
        QByteArray emitter(line.c_str(), line.length()-1);//Drop \0
        emit this->newMeasurement(emitter);
    }
}
