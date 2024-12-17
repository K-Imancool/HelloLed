#include "uartconnect.h"
#include <QDebug>

UartConnect::UartConnect(QObject *parent)
    : QObject{parent},
      m_serial(new QSerialPort(this))
{
    Q_UNUSED(parent);
    openSerialPort();
    connect(m_serial, &QSerialPort::readyRead, this, &UartConnect::readData);
}

UartConnect::~UartConnect()
{
    closeSerialPort();
}

QByteArray UartConnect::tx()
{
    return m_txData;
}

QByteArray UartConnect::rx()
{
    return m_rxData;
}

void UartConnect::openSerialPort()
{
    m_serial->setPortName("ttyS3");
//    m_serial->setPortName("ttyS2");
//    m_serial->setBaudRate(QSerialPort::Baud9600);
    m_serial->setBaudRate(QSerialPort::Baud57600);
    m_serial->setDataBits(QSerialPort::Data8);
    m_serial->setParity(QSerialPort::NoParity);
    m_serial->setStopBits(QSerialPort::OneStop);
    m_serial->setFlowControl(QSerialPort::NoFlowControl);
    if (m_serial->open(QIODevice::ReadWrite)) {

    } else {

    }
}

void UartConnect::closeSerialPort()
{
    if (m_serial->isOpen())
        m_serial->close();
}

void UartConnect::writeData(const QByteArray &data)
{
    qint64 writedByte;
//    if (m_txData != data) {
        m_txData = data;
       writedByte = m_serial->write(m_txData);
        m_serial->waitForBytesWritten(100);
        emit txChanged();
//    }
}

QByteArray UartConnect::readData()
{
    const QByteArray data = m_serial->readAll();
    qDebug() << data;
    if (data != m_rxData) {
        m_rxData = data;
        emit rxChanged();
    }
    return m_rxData;
}

