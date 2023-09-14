#ifdef USE_UART
#include "uartservice.h"
#else
#include "tcpservice.h"
#endif

#include "window.h"
#include <QApplication>



int main(int argc, char** argv)
{
    QApplication app(argc, argv);

#ifdef USE_UART
    UARTService server;
#else
    TCPService server;
#endif

    Window window(&server);
    window.show();

    return app.exec();
}