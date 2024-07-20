#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_serialCOM(new MySerialPort(this))
    , m_serialTypeC(new MySerialPort(this))
    , m_timer(new QTimer(this))
    , m_settings(new QSettings("MyCompany", "ComBridge", this))
{
    ui->setupUi(this);

    connect(m_serialCOM, &QSerialPort::readyRead, this, &MainWindow::readDataFromCOM);
    connect(m_serialTypeC, &QSerialPort::readyRead, this, &MainWindow::readDataFromTypeC);
    connect(m_timer, &QTimer::timeout, this, &MainWindow::checkSerialPorts);

    // 从设置中恢复串口号和波特率
    on_pushButtonRefresh_clicked();

    QString savedComPort = m_settings->value("COMPort").toString();
    QString savedTypeCPort = m_settings->value("TypeCPort").toString();
    int savedBaudRate = m_settings->value("BaudRate").toInt();

    if (!savedComPort.isEmpty() && ui->comboBoxCOM->findText(savedComPort) != -1) {
        ui->comboBoxCOM->setCurrentText(savedComPort);
    }

    if (!savedTypeCPort.isEmpty() && ui->comboBoxTypeC->findText(savedTypeCPort) != -1) {
        ui->comboBoxTypeC->setCurrentText(savedTypeCPort);
    }

    if (savedBaudRate != 0 && ui->comboBoxBaud->findText(QString::number(savedBaudRate)) != -1) {
        ui->comboBoxBaud->setCurrentText(QString::number(savedBaudRate));
    }
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
        m_serialTypeC->setPortName(ui->comboBoxTypeC->currentText());
        int baudRate = ui->comboBoxBaud->currentText().toInt();
        m_serialCOM->setBaudRate(baudRate);
        m_serialTypeC->setBaudRate(baudRate);

        if (!m_serialCOM->open(QIODevice::ReadWrite)) {
            QMessageBox::critical(this, tr("Error"), tr("Failed to open COM port"));
            return;
        }

        if (!m_serialTypeC->open(QIODevice::ReadWrite)) {
            QMessageBox::critical(this, tr("Error"), tr("Failed to open Type-C port"));
            m_serialCOM->close();
            return;
        }

        // 保存当前的串口号和波特率到设置
        m_settings->setValue("COMPort", ui->comboBoxCOM->currentText());
        m_settings->setValue("TypeCPort", ui->comboBoxTypeC->currentText());
        m_settings->setValue("BaudRate", baudRate);

        m_timer->start(500);  // 500ms检查一次串口连接状态
        ui->pushButtonRun->setText(tr("Stop"));
        ui->comboBoxCOM->setEnabled(false);
        ui->comboBoxTypeC->setEnabled(false);
        ui->comboBoxBaud->setEnabled(false);
    } else {
        m_serialCOM->close();
        m_serialTypeC->close();
        m_timer->stop();
        ui->pushButtonRun->setText(tr("Run"));
        ui->comboBoxCOM->setEnabled(true);
        ui->comboBoxTypeC->setEnabled(true);
        ui->comboBoxBaud->setEnabled(true);
    }
}

///
/// \brief MainWindow::readDataFromCOM
///
void MainWindow::readDataFromCOM()
{
    QByteArray data = m_serialCOM->readAll();
    m_serialTypeC->write(data);
}

///
/// \brief MainWindow::readDataFromTypeC
///
void MainWindow::readDataFromTypeC()
{
    QByteArray data = m_serialTypeC->readAll();
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
