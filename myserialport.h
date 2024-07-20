#ifndef MYSERIALPORT_H
#define MYSERIALPORT_H

#include <QSerialPort>

class MySerialPort : public QSerialPort
{
    Q_OBJECT
public:
    explicit MySerialPort(QObject *parent = nullptr);

public slots:
    static QStringList availablePorts();

signals:
};

#endif // MYSERIALPORT_H
