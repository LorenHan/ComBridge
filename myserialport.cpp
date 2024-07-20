#include "myserialport.h"

#include <QtSerialPort>
#include <QDebug>

MySerialPort::MySerialPort(QObject *parent)
    : QSerialPort{parent}
{}

///
/// \brief MySerialPort::availablePorts
/// \return
///
QStringList MySerialPort::availablePorts()
{
    QStringList seriallist;

    for (const QSerialPortInfo &info : QSerialPortInfo::availablePorts()) {
        seriallist.append(info.portName());
    }

    // "COM1", "COM11", "COM13", "COM3", "COM7", "COM9" 排序正确
    std::sort(seriallist.begin(), seriallist.end(), [](const QString &s1, const QString &s2) {
        return s1.mid(3).toInt() < s2.mid(3).toInt();
    });

    return seriallist;
}

