#ifndef UARTCONNECT_H
#define UARTCONNECT_H

#include <QObject>
#include <QSerialPort>
#include <QString>
#include <QTimer>

class UartConnect : public QObject
{
    Q_OBJECT
public:
    explicit UartConnect(QObject *parent = nullptr, QString port = "ttyS3",
                         QSerialPort::BaudRate rate = QSerialPort::Baud57600);
    ~UartConnect();

    QByteArray tx();
    QByteArray rx();

    enum ConnectResult : quint8 {
        CONNECT_ACK = 0,      // Подтверждение получено
        CONNECT_NO,            // Нет ответа
        CONNECT_DATA,            // Пришли какие-то данные
        CONNECT_TX_ERR          // Ошибка передачи
    };

    quint16 getTimeout() const;
    void setTimeout(quint16 newTimeout);

signals:
    void txChanged();
    void rxChanged();

private:
    void openSerialPort();
    void closeSerialPort();

public slots:
    void writeData(const QByteArray &data);
    ConnectResult writeAndRead(QByteArray &txData, QByteArray &rxData);
    QByteArray readData();

private:
    QByteArray m_txData;
    QByteArray m_rxData;
    QString m_portName;
    QSerialPort::BaudRate m_baudRate;
    QSerialPort *m_serial = nullptr;
    quint16 m_timeout;        // Время на получения ответа
    QTimer* m_sendTimer;
};

#endif // UARTCONNECT_H
