#ifndef WINDOW_H
#define WINDOW_H

#include "canvas.h"
#include <QDialog>
#include <QTimer>
#include "comservice.h"

class Window : public QDialog
{
    QTimer timer;
    Canvas canvas;
    COMService *ser{nullptr};

public:
    Window(COMService *com);

    void refresh();
};

#endif