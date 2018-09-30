#ifndef STDINSOURCE_H
#define STDINSOURCE_H

#include <QObject>
#include <QTimerEvent>

class StdInSource : public QObject
{
    Q_OBJECT
public:
    explicit StdInSource(QObject *parent = nullptr);

signals:
    void newMeasurement(QByteArray json);

public slots:
private:
    void timerEvent(QTimerEvent * ev);
};

#endif // STDINSOURCE_H
