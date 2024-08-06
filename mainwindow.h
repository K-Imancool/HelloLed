#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//#include <gpiod.h>
#include <QFileSystemWatcher>
#include <QFile>
#include <QProcess>
#include <QTimer>

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

private:
    Ui::MainWindow *ui;
    QFile *fileGpio;
    QFileSystemWatcher *fileWatcher;
};
#endif // MAINWINDOW_H
