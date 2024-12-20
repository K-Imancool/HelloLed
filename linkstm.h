#ifndef LINKSTM_H
#define LINKSTM_H

#include <QObject>
#include <QTimer>
#include <QThread>
#include "uartconnect.h"

class LinkStm : public QObject
{
    Q_OBJECT
public:
    explicit LinkStm(QObject *parent = nullptr);
    enum TxCommand : quint8 {           // Передаваемые команды
        Allright = 0x00,
        CurrentVersion = 0x01,
        StartTxSoft = 0x81,
        StopTxSoft = 0x82,
        SoftData = 0x83,
        NoTxCommand = 0xFF,
    };
    Q_ENUM(TxCommand);

    enum RxCommand : quint8 {           // Принимаемые команды
        Whatsup = 0x00,
        MyVersion = 0x01,
        ReadyToSoft = 0x81,
        UpdateResult = 0x82,
        SoftDataAck = 0x83,
        NoRxCommand = 0xFF,
    };
    Q_ENUM(RxCommand);

    enum UartState : quint8 {
        STATE_OK = 0,
        STATE_TX_ERR,                   // Ошибка передачи
        STATE_NO_RX,                   // Приёмник не отвечает
        STATE_RX_ERR,                   // Приёмник отвечает не то, что нужно
    };
    Q_ENUM(UartState);

    struct UartTx {
        TxCommand com;
        QByteArray data;
    };

    struct UartRx {
        RxCommand com;
        QByteArray data;
    };
     // Подготовка передачи команды
    QByteArray packTxCommand(UartTx* txCom);
    // Получить строку с шестнадцатиричными данными
    static QString getHexStr(QByteArray byteArray);
    // Расшифровка команды
    UartState unpackRxCommand(QByteArray* rxPacket);

    TxCommand getLastCommand() const;

    const UartState &state() const;

    void setTxCommand(const UartTx &newTxCommand);

    const UartRx &rxCommand() const;

signals:
//    void txError();
//    void rxError();
    void recieveData(UartRx* rxData);
    void error(UartState errorState);

private slots:
    void sendCommand();

private:
    quint16 calculateCrc16(QByteArray* buffer, quint8 len); // Вычисление контрольной суммы
    static const quint8 MAX_PACKET_LEN = 40;                // Длина пакета
    static const quint8 FRAME_START = 0x8A;                // Команды для байт-стаффинга
    static const quint8 FRAME_ESCAPE_CHAR = 0x8B;
    static const quint8 FRAME_XOR_CHAR = 0x20;

private:
    QByteArray* m_txPacket;
    QByteArray* m_rxPacket;
    QTimer* m_uartTimer;
    UartConnect* m_uart;
    QThread* m_uartThread;
    UartState m_state;
    UartTx m_txCommand;
    UartRx m_rxCommand;
};

#endif // LINKSTM_H
