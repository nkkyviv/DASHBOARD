#include "uartservice.h"
#include "setting.h"
#include <QSerialPort>
#include <QDebug>

void UARTService::run(void)
{
    QSerialPort serial;

    serial.setBaudRate(Setting::UART::BAUDRATE);
    serial.setPortName(Setting::UART::PORT);
    serial.setParity(QSerialPort::Parity::NoParity);
    serial.setDataBits(QSerialPort::DataBits::Data8);
    serial.setStopBits(QSerialPort::StopBits::OneStop);
    serial.setFlowControl(QSerialPort::NoFlowControl);

    while (!end)
    {
        if (serial.open(QIODevice::WriteOnly))
        {
            while (!end && serial.isWritable())
            {
                uint8_t temp[sizeof(buffer)]{0};

                {
                    std::scoped_lock<std::mutex> lock(mtx);
                    memcpy(temp, buffer, sizeof(temp));
                }

                if (sizeof(temp) == serial.write(reinterpret_cast<const char *>(temp), sizeof(temp)))
                {
                    if (serial.waitForBytesWritten(Setting::INTERVAL))
                    {
                        status = true;
                        msleep(Setting::INTERVAL / 2);
                    }
                    else
                    {
                        status = false;
                        break;
                    }
                }
                else
                {
                    qDebug() << "Failed to open the port for writing.";
                    status = false;
                    break;
                }
            }
        }

        if (serial.isOpen())
        {
            serial.close();
        }
    }
}