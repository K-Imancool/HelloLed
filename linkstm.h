#ifndef LINKSTM_H
#define LINKSTM_H

#include <QObject>

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

    struct UartCommand {
        quint8 com;
        QByteArray data;
    };

    QByteArray packTxCommand(quint8 command, QByteArray* data); // Подготовка передачи команды

signals:

private:
    quint16 calculateCrc16(QByteArray* buffer, quint8 len); // Вычисление контрольной суммы
    static const quint8 MAX_PACKET_LEN = 40;                // Длина пакета
    static const quint8 FRAME_START = 0x8A;                // Команды для байт-стаффинга
    static const quint8 FRAME_ESCAPE_CHAR = 0x8B;
    static const quint8 FRAME_XOR_CHAR = 0x20;

private:
    QByteArray* txPacket;
    QByteArray* rxPacket;
    TxCommand txCom = NoTxCommand;
    RxCommand rxCom = NoRxCommand;
    QByteArray* txData;
    QByteArray* rxData;
};

#endif // LINKSTM_H
