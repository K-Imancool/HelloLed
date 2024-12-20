#include "uartconnect.h"
#include <QDebug>
#include <QThread>

UartConnect::UartConnect(QObject *parent, QString port, QSerialPort::BaudRate rate)
    : QObject{parent},
      m_portName(port),
      m_baudRate(rate),
      m_serial(new QSerialPort(this)),
      m_timeout(50)
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
    m_serial->setPortName(m_portName);
//    m_serial->setPortName("ttyS2");
//    m_serial->setBaudRate(QSerialPort::Baud9600);
//    m_serial->setBaudRate(QSerialPort::Baud57600);
    m_serial->setBaudRate(m_baudRate);
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

UartConnect::ConnectResult UartConnect::writeAndRead(QByteArray &txData, QByteArray &rxData)
{
    quint16 writedByte;
    writedByte = m_serial->write(txData);
    if (writedByte != txData.size() ||
        !m_serial->waitForBytesWritten(10)) {
        return CONNECT_TX_ERR;
    }
    // Засыпаем, ожидая ответ
    this->thread()->msleep(m_timeout);
    if (m_serial->waitForReadyRead(10)) {
        return CONNECT_NO;
    }

    rxData = m_serial->readAll();
    return CONNECT_ACK;
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

quint16 UartConnect::getTimeout() const
{
    return m_timeout;
}

void UartConnect::setTimeout(quint16 newTimeout)
{
    m_timeout = newTimeout;
}

