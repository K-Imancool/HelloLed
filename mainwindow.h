#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//#include <gpiod.h>
#include <QFileSystemWatcher>
#include <QFile>
#include <QProcess>
#include <QTimer>
#include <QFileSystemModel>
#include <QThread>
#include "linkstm.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnOn_clicked();

    void on_btnOff_clicked();

    void on_btnExit_clicked();

    void changed(const QString& fileName);

    void on_buttonDebug_clicked();

    void on_buttonInfo_clicked();

    void on_buttonWarning_clicked();

    void on_buttonCritical_clicked();

    void on_btnTx_clicked();

    void testDisplay(LinkStm::UartRx rxData);

    void displayUartError(LinkStm::UartState errorState);

    void on_listView_doubleClicked(const QModelIndex &index);

    void on_listView_clicked(const QModelIndex &index);

public slots:
    void displayRxData();

private:
    Ui::MainWindow *ui;
    QFile *fileGpio;
    QFileSystemWatcher *fileWatcher;
    UartConnect* uart;
    QByteArray txPacket;
    QFileSystemModel *fileModel;
    const QString STM_PROJECTS_DIR = "/home/kikorik/stm32Soft";
    QThread *uartThread;
};
#endif // MAINWINDOW_H
