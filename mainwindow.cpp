#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QProcess commandProcess;
    QString commandLine;
    fileGpio = new QFile("/sys/class/gpio/gpio29/value");
    commandLine = "echo kikorik | sudo -S cd /sys/class/gpio; echo 29 > export; echo in > gpio29/direction; echo falling > gpio29/edge;\n";
    commandProcess.start("/bin/bash");
    commandProcess.write(commandLine.toUtf8());
    commandProcess.waitForFinished();
    commandProcess.close();
    fileWatcher = new QFileSystemWatcher(this);
    fileWatcher->addPath(fileGpio->fileName());
    connect(fileWatcher, SIGNAL(fileChanged(QString)), this, SLOT(changed(QString)));

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_btnOn_clicked()
{

}


void MainWindow::on_btnOff_clicked()
{
    QProcess commandProcess;
    QString commandLine;
    commandLine = "echo kikorik | sudo -S poweroff\n";
    commandProcess.start("/bin/bash");
    commandProcess.write(commandLine.toUtf8());
    commandProcess.waitForFinished();
    commandProcess.close();
    ui->label1->setText(QString(commandProcess.readAllStandardError()));
    ui->label1->setText(commandProcess.errorString());
    ui->label1->setText(QString::number(commandProcess.exitCode()));

}


void MainWindow::on_btnExit_clicked()
{
    MainWindow::close();
}

void MainWindow::changed(const QString &fileName)
{
    QFile file(fileName);
    QProcess commandProcess;
    QString commandLine, commandLine2;
    commandLine = "echo kikorik | sudo -S echo default-on > /sys/class/leds/firefly:yellow:user/trigger\n";
    commandLine2 = "echo kikorik | sudo -S echo none > /sys/class/leds/firefly:blue:power/trigger\n";
//    commandLine = "echo kikorik | sudo -S poweroff\n";
    if (file.open(QIODevice::ReadOnly)) {
        QString gpioValue = QString(file.readAll()).trimmed();  // обрезаем, чтобы получить 0 или 1
        ui->btnOff->setText(gpioValue);
        if (gpioValue == "0") {
//            QTimer::singleShot(2000, this, [=]{QProcess::execute("poweroff");});
//            int result = QProcess::execute("echo kikorik | sudo -S poweroff");
            commandProcess.start("/bin/bash");
            commandProcess.write(commandLine.toUtf8());
            commandProcess.waitForFinished();
            commandProcess.write(commandLine2.toUtf8());
            commandProcess.waitForFinished();
            commandProcess.close();
            ui->label1->setText(QString(commandProcess.readAllStandardError()));
            ui->label1->setText(commandProcess.errorString());
//            ui->label1->setText(commandProcess.exitStatus());
            ui->label1->setText(QString::number(commandProcess.exitCode()));
//            ui->btnOn->setText(QString::number(result));
        }
    }
    else {
        ui->btnOn->setText("err");
    }
}



