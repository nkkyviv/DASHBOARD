#ifdef USE_UART
#include "uartservice.h"
#else
#include "tcpservice.h"
#endif

#include "window.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

#ifdef USE_UART
    UARTService service;
#else
    TCPService service;
#endif

    Window mainWindow(&service);
    mainWindow.show();

    return app.exec();
}