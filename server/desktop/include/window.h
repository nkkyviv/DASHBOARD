#ifndef WINDOW_H
#define WINDOW_H

#include <QDialog>
#include <QSlider>
#include <QCheckBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>

#include "comservice.h"
#include <QTimer>
#include <QFileInfo>

class Window : public QDialog
{
    COMService* srv{ nullptr };

public:
    Window(COMService* com);

private:
    QSlider speedSlider;
    QSlider temperatureSlider;
    QSlider batterySlider;

    QLabel speedLabel{ "Speed:" };
    QLabel temperatureLabel{ "Temperature:" };
    QLabel batteryLabel{ "Battery:" };

    QLabel speedValueLabel{ "0 Kph" };
    QLabel temperatureValueLabel{ "0 °C" };
    QLabel batteryValueLabel{ "0%" };

    QLabel LightSignals{ "Light Signals:" };
    QCheckBox leftCheckBox{ "Left" };
    QCheckBox rightCheckBox{ "Right" };
    QCheckBox warningCheckBox{ "Warning" };

    QGridLayout mainLayout;
    QHBoxLayout signalsLayout;
};

#endif