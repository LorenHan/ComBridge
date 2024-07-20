#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>
#include <QMessageBox>

#include "myserialport.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButtonRun_clicked();
    void on_pushButtonRefresh_clicked();
    void readDataFromCOM();
    void readDataFromTypeC();
    void checkSerialPorts();

private:
    Ui::MainWindow *ui;
    MySerialPort *m_serialCOM;  // 测试设备连接的串口
    MySerialPort *serialTypeC;  // 驱动器连接的串口
    QTimer *m_timer;           // 定时器

protected:
    void showEvent(QShowEvent *event);
};

#endif // MAINWINDOW_H
