#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
    if (ui->pushButtonRun->text() == "Run") {
        ui->pushButtonRun->setText("Stop");
    } else {
        ui->pushButtonRun->setText("Run");
    }
}

