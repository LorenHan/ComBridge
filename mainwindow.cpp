#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_serialCOM(new MySerialPort(this))
    , serialTypeC(new MySerialPort(this))
    , m_timer(new QTimer(this))
{
    ui->setupUi(this);

    connect(m_serialCOM, &QSerialPort::readyRead, this, &MainWindow::readDataFromCOM);
    connect(serialTypeC, &QSerialPort::readyRead, this, &MainWindow::readDataFromTypeC);
    connect(m_timer, &QTimer::timeout, this, &MainWindow::checkSerialPorts);
}

MainWindow::~MainWindow()
{
    delete ui;
}

///
/// \brief MainWindow::on_pushButtonRun_clicked
///
void MainWindow::on_pushButtonRun_clicked()
{
    if (ui->pushButtonRun->text() == tr("Run")) {
        m_serialCOM->setPortName(ui->comboBoxCOM->currentText());
        serialTypeC->setPortName(ui->comboBoxTypeC->currentText());

        if (!m_serialCOM->open(QIODevice::ReadWrite)) {
            qDebug() << tr("Failed to open COM port");
            return;
        }

        if (!serialTypeC->open(QIODevice::ReadWrite)) {
            qDebug() << tr("Failed to open Type-C port");
            m_serialCOM->close();
            return;
        }

        m_timer->start(1000);  // 每秒检查一次串口状态
        ui->pushButtonRun->setText(tr("Stop"));
    } else {
        m_serialCOM->close();
        serialTypeC->close();
        m_timer->stop();
        ui->pushButtonRun->setText(tr("Run"));
    }
}

///
/// \brief MainWindow::readDataFromCOM
///
void MainWindow::readDataFromCOM()
{
    QByteArray data = m_serialCOM->readAll();
    serialTypeC->write(data);
}

///
/// \brief MainWindow::readDataFromTypeC
///
void MainWindow::readDataFromTypeC()
{
    QByteArray data = serialTypeC->readAll();
    m_serialCOM->write(data);
}

///
/// \brief MainWindow::on_pushButtonRefresh_clicked
///
void MainWindow::on_pushButtonRefresh_clicked()
{
    ui->comboBoxCOM->clear();
    ui->comboBoxTypeC->clear();

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        ui->comboBoxCOM->addItem(info.portName());
        ui->comboBoxTypeC->addItem(info.portName());
    }
}

///
/// \brief MainWindow::showEvent
/// \param event
///
void MainWindow::showEvent(QShowEvent *event)
{
    QMainWindow::showEvent(event);
    on_pushButtonRefresh_clicked();
}

///
/// \brief MainWindow::checkSerialPorts
///
void MainWindow::checkSerialPorts()
{
    if (ui->pushButtonRun->text() == tr("Stop")) {
        QStringList comPorts = MySerialPort::availablePorts();
        QString comPort = ui->comboBoxCOM->currentText();
        QString typeCPort = ui->comboBoxTypeC->currentText();

        if (!comPorts.contains(comPort)) {
            QMessageBox::warning(this, tr("Warning"), tr("COM port is disconnected"));
            on_pushButtonRun_clicked();
        }

        if (!comPorts.contains(typeCPort)) {
            QMessageBox::warning(this, tr("Warning"), tr("Type-C port is disconnected"));
            on_pushButtonRun_clicked();
        }
    }
}
