#ifndef UARTCONNECT_H
#define UARTCONNECT_H

#include <QObject>
#include <QSerialPort>
#include <QString>

class UartConnect : public QObject
{
    Q_OBJECT
public:
    explicit UartConnect(QObject *parent = nullptr);
    ~UartConnect();

    QByteArray tx();
    QByteArray rx();

signals:
    void txChanged();
    void rxChanged();

private:
    void openSerialPort();
    void closeSerialPort();

public slots:
    void writeData(const QByteArray &data);
    QByteArray readData();

private:
    QSerialPort *m_serial = nullptr;
    QByteArray m_txData;
    QByteArray m_rxData;
};

#endif // UARTCONNECT_H
