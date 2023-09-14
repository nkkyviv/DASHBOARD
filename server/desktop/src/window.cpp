#include "window.h"
#include "setting.h"
#include "comservice.h"


Window::Window(COMService* com) : srv{ com }
{
    setWindowFlags(Qt::WindowStaysOnTopHint);
    setWindowTitle("Server");
    setFixedHeight(Setting::Server::Window::Height);
    setFixedWidth(Setting::Server::Window::Width);

    setLayout(&mainLayout);

    mainLayout.addWidget(&speedLabel, 0, 0);
    speedLabel.setAlignment(Qt::AlignRight);
    mainLayout.addWidget(&speedSlider, 0, 1);
    mainLayout.addWidget(&speedValueLabel, 0, 2);
    speedValueLabel.setFixedSize(55, 17);

    mainLayout.addWidget(&temperatureLabel, 1, 0);
    temperatureLabel.setAlignment(Qt::AlignRight);
    mainLayout.addWidget(&temperatureSlider, 1, 1);
    mainLayout.addWidget(&temperatureValueLabel, 1, 2);

    mainLayout.addWidget(&batteryLabel, 2, 0);
    batteryLabel.setAlignment(Qt::AlignRight);
    mainLayout.addWidget(&batterySlider, 2, 1);
    mainLayout.addWidget(&batteryValueLabel, 2, 2);

    mainLayout.addWidget(&LightSignals, 3, 0);

    signalsLayout.addWidget(&leftCheckBox);
    signalsLayout.addWidget(&rightCheckBox);
    signalsLayout.addWidget(&warningCheckBox);

    mainLayout.addLayout(&signalsLayout, 3, 1);
    speedSlider.setOrientation(Qt::Horizontal);
    speedSlider.setRange(0, 240);

    temperatureSlider.setOrientation(Qt::Horizontal);
    temperatureSlider.setRange(-60, 60);

    batterySlider.setOrientation(Qt::Horizontal);
    batterySlider.setRange(0, 100);





    connect(&speedSlider, &QSlider::valueChanged, this, [this](int value)
        { QString str;
    srv->setSpeed(value);
    str.setNum(value);
    speedValueLabel.setText(str + " Kph"); });

    connect(&temperatureSlider, &QSlider::valueChanged, this, [this](int value)
        {
            QString str;
            srv->setTemperature(value);
            str.setNum(value);
            temperatureValueLabel.setText(str + " °C"); });

    connect(&batterySlider, &QSlider::valueChanged, this, [this](int value)
        {
            QString str;
            srv->setBatteryLevel(value);
            str.setNum(value);
            batteryValueLabel.setText(str + "%"); });

    connect(&leftCheckBox, &QCheckBox::stateChanged, this, [this](int state)
        {

            srv->setLightLeft(state);
            if (warningCheckBox.isChecked()) {
                srv->setLightLeft(true);
                srv->setLightRight(true);
            }
            else {
                if (state == Qt::Unchecked && !rightCheckBox.isChecked()) {
                    srv->setLightRight(false);
                }
            }
            rightCheckBox.setEnabled(state == 0); });

    connect(&rightCheckBox, &QCheckBox::stateChanged, this, [this](int state)
        {

            srv->setLightRight(state);
            if (warningCheckBox.isChecked()) {
                srv->setLightLeft(true);
                srv->setLightRight(true);
            }
            else {
                if (state == Qt::Unchecked && !leftCheckBox.isChecked()) {
                    srv->setLightLeft(false);
                }
            }
            leftCheckBox.setEnabled(state == 0); });

    connect(&warningCheckBox, &QCheckBox::stateChanged, this, [this](int state)
        {

            if (state == Qt::Unchecked) {
                srv->setLightLeft(leftCheckBox.isChecked());
                srv->setLightRight(rightCheckBox.isChecked());
            }
            else {
                srv->setLightLeft(true);
                srv->setLightRight(true);
            } });
}
