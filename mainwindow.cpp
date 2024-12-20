#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QThread"
#include "QDebug"
#include "QtNetwork/QNetworkConfigurationManager"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>
#include <QTime>
#include <QMetaEnum>

#include <QDir>

#include "LoggingCategories.h"
#include "linkstm.h"


QSerialPort* serial;
QTimer* timer;
LinkStm* linkStm;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    QThread::sleep(2);
//    fileGpio = new QFile("/home/kikorik/tempShit");
    qInfo(logInfo()) << "Включение";
    fileGpio = new QFile("/sys/class/gpio/gpio29/value");
    fileWatcher = new QFileSystemWatcher(this);
    fileWatcher->addPath(fileGpio->fileName());
    connect(fileWatcher, SIGNAL(fileChanged(QString)), this, SLOT(changed(QString)));

//    QFont font = ui->label4->font();
//    font.setPixelSize(150);
    ui->labelRx->setStyleSheet("color: #802070; font-size: 50px");

//    QNetworkConfigurationManager netManager;
//    foreach (const QNetworkConfiguration& netConfig, netManager.allConfigurations()) {
//      if (netConfig.bearerType() == QNetworkConfiguration::BearerWLAN) {
//        // I'm wireless network
//        if (netConfig.type() == QNetworkConfiguration::InternetAccessPoint) {
//          // I'm access point
//        }
//      }
//    }

//    QNetworkConfiguration cfg;
//    QNetworkConfigurationManager ncm;
//    auto nc = ncm.allConfigurations();

//    for (auto &x : nc)
//    {
//    if (x.bearerType() == QNetworkConfiguration::BearerWLAN)
//    {
//    qDebug() << x.name();
//    if (x.name() == "YouDesiredNetwork")
//    cfg = x;
//    }
//    }
//    auto session = new QNetworkSession(cfg, this);
//    session->open();
    // Класс для связи с stm по uart
    linkStm = new LinkStm();
    // Привязываем сигналы
    QObject::connect(linkStm, &LinkStm::recieveData, this, &MainWindow::testDisplay);
    QObject::connect(linkStm, &LinkStm::error, this, &MainWindow::displayUartError);

    serial = new QSerialPort(this);
    QString tmpStr;
    foreach(QSerialPortInfo port, QSerialPortInfo::availablePorts())
    {
       tmpStr.append(port.portName()).append(", ");
    }
    ui->label1->setText(tmpStr);



//    timer = new QTimer();
//    connect(timer, &QTimer::timeout, this, &MainWindow::on_btnTx_clicked);
//    timer->start(1000);

    fileModel = new QFileSystemModel(this);
    fileModel->setFilter(QDir::AllEntries);
    fileModel->setRootPath(STM_PROJECTS_DIR);
//    fileModel->setRootPath(QDir::currentPath());
    ui->listView->setModel(fileModel);
    ui->listView->setRootIndex(fileModel->index(STM_PROJECTS_DIR));
    ui->treeView->setModel(fileModel);
    ui->treeView->setRootIndex(fileModel->index(STM_PROJECTS_DIR));
    ui->label2->setText(STM_PROJECTS_DIR);
    ui->label3->setText(QDir::currentPath());
    connect(ui->listView, &QListView::clicked, this, &MainWindow::on_listView_clicked);
}

MainWindow::~MainWindow()
{
    uartThread->quit();         // Вырубаем поток при выходе
    uartThread->wait(1000);
    delete ui;
}


void MainWindow::on_btnOn_clicked()
{
//    system("touch /home/kikorik/testfile");
    static bool isRed = false;

    QProcess commandProcess;
    QString command;    //Contains the command to be executed
    QStringList args;   //Contains arguments of the command

    if (!isRed) {
        command = "echo kikorik | sudo -S bash -c \"echo default-on > /sys/class/leds/firefly:yellow:user/trigger\"";
        isRed = true;
        ui->label2->setText("Purple");
    }
    else {
        command = "echo kikorik | sudo -S bash -c \"echo none > /sys/class/leds/firefly:yellow:user/trigger\"";
        isRed = false;
        ui->label2->setText("Blue");
    }
    commandProcess.start(command,args,QIODevice::ReadOnly); //Starts execution of command
    commandProcess.waitForFinished();                       //Waits for execution to complete

    QString stdOut      =   commandProcess.readAllStandardOutput();  //Reads standard output
    QString stdError    =   commandProcess.readAllStandardError();   //Reads standard error

//    QProcess commandProcess;
//    QString commandLine;
//    commandLine = "echo kikorik | sudo -S poweroff\n";
//    commandProcess.start("/bin/bash");
//    commandProcess.write(commandLine.toUtf8());
//    commandProcess.waitForFinished();
    commandProcess.close();
//    ui->label2->setText("Out: " + stdOut);
//    ui->label3->setText("Err: " + stdError);
}


void MainWindow::on_btnOff_clicked()
{
    QProcess commandProcess;
    QString command = "bash";    //Contains the command to be executed
    QStringList args;   //Contains arguments of the command
    args << "-c" <<"echo kikorik | sudo -S poweroff";

    commandProcess.start(command,args,QIODevice::ReadOnly); //Starts execution of command
    commandProcess.waitForFinished();                       //Waits for execution to complete

    QString stdOut      =   commandProcess.readAllStandardOutput();  //Reads standard output
    QString stdError    =   commandProcess.readAllStandardError();   //Reads standard error

    commandProcess.close();
//    ui->label4->setText(QString::number(commandProcess.exitCode()));
}


void MainWindow::on_btnExit_clicked()
{
    MainWindow::close();
}

void MainWindow::changed(const QString &fileName)
{
    static int check0 = 0, check1 = 0;
    QFile file(fileName);
//    commandLine = "echo kikorik | sudo -S poweroff\n";
    if (file.open(QIODevice::ReadOnly)) {
        QString fileContent = QString(file.readAll());  // Считываем содержимое файла value
        qDebug(logDebug()) << fileContent;
        QString gpioValue = fileContent.trimmed();  // обрезаем, чтобы получить 0 или 1 (лог. уровень на GPIO0_D5)
        ui->labelRx->setText(gpioValue);
//        QThread::sleep(2);
        if (gpioValue == "0") {
            qInfo(logInfo()) << "Питание отключено" << QString::number(++check0);
//            qCritical(logCritical()) << "Питание отключено";
            ui->label2->setText(QString::number(++check0));
            ui->btnOn->setText("0");
    //            QThread::sleep(2);
            QProcess commandProcess;
//            QString command = "touch";    //Contains the command to be executed
            QString command = "bash";    //Contains the command to be executed
            QStringList args;   //Contains arguments of the command
//            args << "/cd/home/kikorik/tempInterr0";
            args << "-c" <<"echo kikorik | sudo -S poweroff";
            commandProcess.start(command,args,QIODevice::ReadOnly); //Starts execution of command
            commandProcess.waitForFinished();                       //Waits for execution to complete
            commandProcess.close();
            QApplication::exit();
        }
        else if (gpioValue == "1") {
            ui->label3->setText(QString::number(++check1));
            ui->btnOff->setText("1");
        }
    }
    else {
        ui->label1->setText("Error opening file");
    }
}




void MainWindow::on_buttonDebug_clicked()       // Очистка лога
{
    QFile log("/home/kikorik/OnyxLog/logFile.txt");
    if (log.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
       log.close();
    }
    qDebug(logDebug()) << "Лог очищен";
}


void MainWindow::on_buttonInfo_clicked()
{
    qInfo(logInfo()) << "Info Button";
}


void MainWindow::on_buttonWarning_clicked()
{
    qWarning(logWarning()) << "Warning Button";
}


void MainWindow::on_buttonCritical_clicked()
{
    qCritical(logCritical()) << "Critical Button";
}


void MainWindow::on_btnTx_clicked()
{
    QString txDataStr = "TX: ";

    QByteArray txData("ABCD");
    txPacket = linkStm->packTxCommand(LinkStm::CurrentVersion, &txData);

//    static int16_t counter = 0;
//    QByteArray txData;
//    txPacket.resize(9);
//    txPacket[0] = 0x8A;
//    txPacket[1] = 0x08;
//    txPacket[2] = 0xCC;
//    txPacket[3] = 0x12;
//    txPacket[4] = 0x34;
//    txPacket[5] = 0x56;
//    txPacket[6] = 0x78;
//    txPacket[7] = 0x0F;
//    txPacket[8] = 0xED;

    txDataStr.append(LinkStm::getHexStr(txPacket));
    ui->label2->setText(txDataStr);
    uart->writeData(txPacket);
    ui->labelFlag->clear();
}

void MainWindow::testDisplay(LinkStm::UartRx rxData)
{
    QString uartString = QString::number(rxData.com, 16);

    uartString.append(": ");
    uartString.append(linkStm->getHexStr(rxData.data));

    ui->label3->setText(uartString);
}

void MainWindow::displayUartError(LinkStm::UartState errorState)
{
    QMetaEnum metaEnum = QMetaEnum::fromType<LinkStm::UartState>();
    ui->labelFlag->setText(metaEnum.valueToKey(errorState));
}

void MainWindow::displayRxData()
{
    QByteArray rxArray = uart->rx();
    QString rxData;
    for (int i = 0; i < rxArray.size(); i++) {
        QString codeHex = QString::number(rxArray.at(i), 16);
        rxData.append(codeHex).append(" ");
    }
//    QString rxData = QString::number(rxArray[0], 16);
    ui->labelRx->setText(rxData);
    ui->labelFlag->setText("Something");
}


void MainWindow::on_listView_doubleClicked(const QModelIndex &index)
{
    QListView* listView = (QListView*)sender();
    QFileInfo fileInfo = fileModel->fileInfo(index);
    if (fileInfo.fileName() == "..") {
        QDir dir = fileInfo.dir();
        dir.cdUp();
        listView->setRootIndex(fileModel->index(dir.absolutePath()));
    }
    else if (fileInfo.fileName() == ".") {
        listView->setRootIndex(fileModel->index(STM_PROJECTS_DIR));
    }
    else if (fileInfo.isDir()) {
        listView->setRootIndex(index);
    }
}


void MainWindow::on_listView_clicked(const QModelIndex &index)
{
    QListView* listView = (QListView*)sender();
    QFileInfo fileInfo = fileModel->fileInfo(index);
    if (fileInfo.fileName() == "..") {
        QDir dir = fileInfo.dir();
        dir.cdUp();
        listView->setRootIndex(fileModel->index(dir.absolutePath()));
    }
    else if (fileInfo.fileName() == ".") {
        listView->setRootIndex(fileModel->index(STM_PROJECTS_DIR));
    }
    else if (fileInfo.isDir()) {
        listView->setRootIndex(index);
    }

    ui->label3->setText(fileInfo.fileName());
}

