#include "window.h"
#include "setting.h"
#include <QTimer>
#include <QGridLayout>
#include <QPainter>
#include <QRandomGenerator>

Window::Window(COMService *com) : ser{com}
{
    setWindowFlags(Qt::WindowStaysOnTopHint);
    setWindowTitle("Client");
    setFixedHeight(560);
    setFixedWidth(800);
    setContentsMargins(0, 0, 0, 0);

    QGridLayout layout(this);

    layout.addWidget(&canvas, 0, 0, 1, 3);
    layout.setContentsMargins(0, 0, 0, 0);

    setLayout(&layout);

    canvas.setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    canvas.setFixedSize(800, 560);

    // Connect the timer's timeout signal to the refresh slot
    connect(&timer, &QTimer::timeout, this, &Window::refresh);

    // Set the timer interval (in milliseconds)
    timer.start(Setting::INTERVAL); // Refresh every 1000 ms (1 second)
}

void Window::refresh()
{
    if (ser->getStatus())
    {
        canvas.setStatus(true);
        canvas.setSpeed(ser->getSpeed());
        canvas.setTemperatureLevel(ser->getTemperature());
        canvas.setBatteryLevel(ser->getBatteryLevel());
        canvas.setLightSignal(ser->getLightRight(), ser->getLightLeft());
    }
    else
    {
        canvas.setSpeed(0);
        canvas.setStatus(false);
        canvas.setBatteryLevel(0);
        canvas.setTemperatureLevel(0);
        canvas.setLightSignal(false, false);
    }

    canvas.update();
}