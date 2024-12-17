#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//#include <gpiod.h>
#include <QFileSystemWatcher>
#include <QFile>
#include <QProcess>
#include <QTimer>
#include "uartconnect.h"

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

    void testDisplay();

public slots:
    void displayRxData();

private:
    Ui::MainWindow *ui;
    QFile *fileGpio;
    QFileSystemWatcher *fileWatcher;
    UartConnect* uart;
    QByteArray txPacket;
};
#endif // MAINWINDOW_H
