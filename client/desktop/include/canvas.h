#ifndef CANVAS_H
#define CANVAS_H

#include <QFont>
#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QSoundEffect>

class Canvas : public QWidget
{
    QPen pen;
    QBrush brush;
    QPainter painter;
    QFont labelFont{"Arial", 12, QFont::Normal};
    QFont speedFont{"Arial", 18, QFont::Normal};
    QFont iconFont{"Material Icons", 8, QFont::Normal};

    QSoundEffect lightSoundEffect;

    void playSound(QSoundEffect &soundEffect);
    void stopSound(QSoundEffect &soundEffect);

    int speed{0};
    bool status{false};
    int batteryLevel{0};
    int temperatureLevel{0};
    bool isLeftArrowVisible{false};
    bool isRightArrowVisible{false};
    bool isWarningVisible{false};

public:
    Canvas();

    void setSpeed(int spd) { speed = spd; }
    void setStatus(bool stts) { status = stts; }
    void setBatteryLevel(int percent) { batteryLevel = percent; }
    void setTemperatureLevel(int degree) { temperatureLevel = degree; }
    void setLightSignal(bool visible_R, bool visible_L)
    {
        isRightArrowVisible = visible_R;
        isLeftArrowVisible = visible_L;
    }

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void drawBatteryLevel(void);
    void drawTemperatureLevel(void);
    void drawLightSignal(void);
    void drawSpeed(void);
    void drawSpeedometerNeedle(void);
};

#endif