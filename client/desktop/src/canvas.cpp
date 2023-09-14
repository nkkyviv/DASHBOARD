#include "canvas.h"
#include <QFileInfo>
#include <QPaintEvent>
#include <QMediaDevices>
#include <QAudioDevice>
#include <QFontDatabase>

Canvas::Canvas()
{
    QFileInfo file{__FILE__};
    QString resDir = file.absolutePath() + "/../res/";

    QFontDatabase::addApplicationFont(resDir + "font.ttf");

    lightSoundEffect.setAudioDevice(QMediaDevices::defaultAudioOutput());
    lightSoundEffect.setSource(QUrl::fromLocalFile(resDir + "turn-signals.wav"));
    lightSoundEffect.setVolume(1);
    lightSoundEffect.setLoopCount(QSoundEffect::Infinite);
}

void Canvas::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event); // Call the base class paintEvent implementation

    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Set the background color using a QBrush
    painter.fillRect(event->rect(), QColor(64, 32, 64)); // Red: 64, Green: 32, Blue: 64
    painter.setBrush(brush);
    painter.setPen(pen);

    drawBatteryLevel();
    drawTemperatureLevel();
    drawLightSignal();
    drawSpeed();
    drawSpeedometerNeedle();

    painter.end();
}
void Canvas::drawBatteryLevel(void)
{
    iconFont.setPixelSize(110);
    painter.setFont(iconFont);

    int iconWidth = 200;
    int originalIconHeight = 220; // The original icon height
    int newIconHeight = 200;      // Increase this value to make the icon taller

    int xOffset = width() - iconWidth + 50;
    int yOffset = height() - originalIconHeight - 100;

    int batteryHeight = static_cast<int>(70 * (static_cast<double>(batteryLevel) / 100.0));

    if (batteryLevel < 25)
    {
        painter.setPen(Qt::red);
        QColor batteryFillColor = Qt::red;
        painter.setBrush(batteryFillColor);
    }
    else if (batteryLevel >= 25 && batteryLevel <= 49)
    {
        painter.setPen(Qt::yellow);
        QColor batteryFillColor = Qt::yellow;
        painter.setBrush(batteryFillColor);
    }
    else
    {
        painter.setPen(Qt::green);
        QColor batteryFillColor = Qt::green;
        painter.setBrush(batteryFillColor);
    }

    QRect batteryRect(xOffset + 85, 335 + (45 - batteryHeight), 30, batteryHeight);
    painter.drawRect(batteryRect);

    painter.setFont(iconFont); // Reset the font for drawing the icon
    painter.drawText(QRect(xOffset, yOffset, iconWidth, newIconHeight), Qt::AlignCenter, QString::fromUtf8("\uebdc"));

    int textX = xOffset - 30;
    int textY = yOffset + newIconHeight - 50;
    QString batteryText = QString::number(batteryLevel) + "%";
    painter.setFont(labelFont);
    painter.setPen(Qt::white);
    painter.drawText(QRect(textX, textY, iconWidth + 60, 20), Qt::AlignCenter, batteryText);
}

void Canvas::drawTemperatureLevel(void)
{
    iconFont.setPixelSize(50);
    painter.setFont(iconFont);
    // Calculate position
    int iconWidth = 50;                       // Adjust the width of the icon as needed
    int iconHeight = 50;                      // Adjust the height of the icon as needed
    int xOffset = width() - iconWidth - 25;   // Adjust the offset as needed
    int yOffset = height() - iconHeight - 50; // Adjust the offset as needed
    // Determine temperature color based on temperature value
    QColor tempColor;
    if (temperatureLevel < 5)
    {
        tempColor = Qt::white;
    }
    else if (temperatureLevel >= 5 && temperatureLevel <= 39)
    {
        tempColor = Qt::blue;
    }
    else
    {
        tempColor = Qt::red;
    }
    painter.setPen(tempColor); // Set the pen color for drawing text
    painter.drawText(QRect(xOffset, yOffset, iconWidth, iconHeight), Qt::AlignCenter, QString::fromUtf8("\ue1ff"));

    // Calculate position for the text
    int textX = xOffset - 30;             // Adjust the X position as needed
    int textY = yOffset + iconHeight - 5; // Adjust the Y position as needed
    QString tempText = QString::number(temperatureLevel) + " °C";
    painter.setFont(labelFont);
    painter.setPen(Qt::white); // Reset the pen color for value text
    painter.drawText(QRect(textX, textY + 5, iconWidth + 60, 20), Qt::AlignCenter, tempText);
}

void Canvas::drawLightSignal(void)
{
    iconFont.setPixelSize(50);
    painter.setFont(iconFont);

    // Calculate position for the warning signal
    int iconWidth = 50;  // Adjust the width of the icon as needed
    int iconHeight = 50; // Adjust the height of the icon as needed
    int xOffset = 25;    // Adjust the offset as needed
    int yOffset = 50;

    // Calculate position for the arrows
    int arrowWidth = 50;                                // Adjust the width of the arrow icon as needed
    int arrowHeight = 50;                               // Adjust the height of the arrow icon as needed
    int leftArrowXOffset = 25;                          // Adjust the left arrow offset as needed
    int rightArrowXOffset = width() - arrowWidth - 100; // Adjust the right arrow offset as needed
    int arrowYOffset = 50;                              // Adjust the offset as needed

    constexpr int period = 20;
    static uint8_t counter{0};
    static bool islightSoundEffect = false;

    counter++;

    if (counter >= period)
    {
        counter = 0;
    }

    QPen greenPen(Qt::green);

    if (counter < (period / 2))
    {
        if (isLeftArrowVisible || isRightArrowVisible)
        {
            if (!islightSoundEffect)
            {
                lightSoundEffect.play();
                islightSoundEffect = true;
            }

            if (isLeftArrowVisible)
            {
                painter.setPen(greenPen);
                painter.drawText(QRect(leftArrowXOffset, arrowYOffset, arrowWidth, arrowHeight), Qt::AlignCenter, QString::fromUtf8("\ue5c4"));
            }

            if (isRightArrowVisible)
            {
                painter.setPen(greenPen);
                painter.drawText(QRect(rightArrowXOffset, arrowYOffset, arrowWidth, arrowHeight), Qt::AlignCenter, QString::fromUtf8("\ue5c8"));
            }
        }
        else
        {
            if (islightSoundEffect)
            {
                lightSoundEffect.stop();
                islightSoundEffect = false;
            }
        }
    }
}

void Canvas::drawSpeed(void)
{
    int centerX = width() / 2 * 0.9;  // Calculate the center X-coordinate of the canvas
    int centerY = height() / 2 * 1.2; // Calculate the center Y-coordinate of the canvas

    // Calculate the radius of the circle to fit within the canvas
    int radius = qMin(width(), height()) * 0.55; // Adjust the scale factor as needed

    int startAngle = -40 * 16; // Start angle in degrees * 16 (Qt uses 16-bit fixed point angles)
    int endAngle = 220 * 16;   // End angle in degrees * 16

    int startA = -37 * 16; // Start angle for the markings
    int endA = 217 * 16;   // End angle for the markings

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    // Set up the pen for drawing the arc outline
    QPen pen(Qt::white);        // Set any color you prefer
    int arcThickness = 8;       // Thickness of the arc outline in pixels
    pen.setWidth(arcThickness); // Adjust the pen width as needed
    painter.setPen(pen);

    painter.drawArc(centerX - radius, centerY - radius, radius * 2, radius * 2, startAngle, endAngle - startAngle);

    // Draw main markings at evenly spaced intervals
    int numMainMarkings = 13;
    int numSubMarkings = 1;          // Adjust the number of sub-markings
    int markingThickness = 7;        // Adjust the thickness of the markings
    int markingRadius = radius - 25; // Adjust the radius where markings are drawn
    int markingLength = 25;

    pen.setWidth(markingThickness); // Thickness of the markings outline in pixels
    qreal angleIncrement = static_cast<qreal>(endA - startA) / (numMainMarkings - 1);

    for (int i = 0; i < numMainMarkings; ++i)
    {
        qreal angle = startA + i * angleIncrement;
        qreal angleRadians = qDegreesToRadians(angle / 16.0);

        qreal startX = centerX + (radius - 10) * qCos(angleRadians);
        qreal startY = centerY - (radius - 10) * qSin(angleRadians);
        qreal endX = centerX + (radius - markingLength) * qCos(angleRadians);
        qreal endY = centerY - (radius - markingLength) * qSin(angleRadians);

        painter.setPen(QPen(Qt::white, markingThickness));
        painter.drawLine(QPointF(startX, startY), QPointF(endX, endY));

        // Draw sub-markings
        if (i != numMainMarkings - 1)
        {
            qreal subMarkingAngleIncrement = (endAngle - startAngle) / (numMainMarkings - 1) / (numSubMarkings + 1);
            qreal subMarkingAngle = angle + subMarkingAngleIncrement;

            for (int j = 0; j < numSubMarkings; ++j)
            {
                qreal subMarkingStartX = centerX + markingRadius * qCos(qDegreesToRadians(subMarkingAngle / 16.0));
                qreal subMarkingStartY = centerY - markingRadius * qSin(qDegreesToRadians(subMarkingAngle / 16.0));
                qreal subMarkingEndX = subMarkingStartX + markingLength * 0.5 * qCos(qDegreesToRadians(subMarkingAngle / 16.0));
                qreal subMarkingEndY = subMarkingStartY - markingLength * 0.5 * qSin(qDegreesToRadians(subMarkingAngle / 16.0));

                painter.setPen(QPen(Qt::white, 3));
                painter.drawLine(QPointF(subMarkingStartX, subMarkingStartY), QPointF(subMarkingEndX, subMarkingEndY));

                subMarkingAngle += subMarkingAngleIncrement;

                // Draw sub-sub-markings between main and sub markings
                qreal subSubMarkingAngleIncrement = subMarkingAngleIncrement / (numSubMarkings + 1);
                qreal subSubMarkingAngle = subMarkingAngle - subSubMarkingAngleIncrement;
                qreal subSubMarkingAngle2 = angle + subSubMarkingAngleIncrement;

                for (int k = 0; k < numSubMarkings; ++k)
                {
                    qreal subSubMarkingStartX = centerX + markingRadius * qCos(qDegreesToRadians(subSubMarkingAngle / 16.0));
                    qreal subSubMarkingStartY = centerY - markingRadius * qSin(qDegreesToRadians(subSubMarkingAngle / 16.0));
                    qreal subSubMarkingEndX = subSubMarkingStartX + markingLength * 0.25 * qCos(qDegreesToRadians(subSubMarkingAngle / 16.0));
                    qreal subSubMarkingEndY = subSubMarkingStartY - markingLength * 0.25 * qSin(qDegreesToRadians(subSubMarkingAngle / 16.0));

                    painter.setPen(QPen(Qt::white, 2));
                    painter.drawLine(QPointF(subSubMarkingStartX, subSubMarkingStartY), QPointF(subSubMarkingEndX, subSubMarkingEndY));

                    subSubMarkingAngle -= subSubMarkingAngleIncrement;

                    qreal subSubMarkingStartX2 = centerX + markingRadius * qCos(qDegreesToRadians(subSubMarkingAngle2 / 16.0));
                    qreal subSubMarkingStartY2 = centerY - markingRadius * qSin(qDegreesToRadians(subSubMarkingAngle2 / 16.0));
                    qreal subSubMarkingEndX2 = subSubMarkingStartX2 + markingLength * 0.25 * qCos(qDegreesToRadians(subSubMarkingAngle2 / 16.0));
                    qreal subSubMarkingEndY2 = subSubMarkingStartY2 - markingLength * 0.25 * qSin(qDegreesToRadians(subSubMarkingAngle2 / 16.0));

                    painter.setPen(QPen(Qt::white, 2));
                    painter.drawLine(QPointF(subSubMarkingStartX2, subSubMarkingStartY2), QPointF(subSubMarkingEndX2, subSubMarkingEndY2));

                    subSubMarkingAngle2 += subSubMarkingAngleIncrement;

                    // Draw sub-sub-markings
                    qreal subSubMarkingAngleIncrement = subMarkingAngleIncrement / (numSubMarkings + 1);
                    qreal subSubMarkingAngle = subMarkingAngle + subSubMarkingAngleIncrement;
                    qreal subSubMarkingAngle2 = subMarkingAngle - subSubMarkingAngleIncrement;
                }
            }
        }
    }

    // Draw the integer labels for the speedometer
    int labelRadius = radius - 65; // Adjust the radius where labels are drawn
    int labelPadding = 10;         // Assign a value to labelPadding

    for (int speed = 0; speed <= 240; speed += 20)
    {
        qreal angle = startA + (endA - startA) * (1.0 - (speed / 240.0)); // Invert the angle calculation
        qreal angleRadians = qDegreesToRadians(angle / 16.0);

        QString label = QString::number(speed);
        QFont labelFont("Arial", 18, QFont::Normal);
        QFontMetrics labelMetrics(labelFont);
        QRect labelRect = labelMetrics.boundingRect(label);

        qreal labelX = centerX + (labelRadius + labelPadding) * qCos(angleRadians) - labelRect.width() / 2.0;
        qreal labelY = centerY - (labelRadius + labelPadding) * qSin(angleRadians) + labelRect.height() / 4.0;

        painter.setFont(labelFont);
        painter.setPen(QPen(Qt::white));
        painter.drawText(labelX, labelY, label);
    }

    // Calculate position for the speed text
    int textX = width() / 2 - 80; // Adjust the X position as needed
    int textY = height() - 110;   // Adjust the Y position as needed

    // Calculate position for the speed logo
    int logoX = textX - 6;   // Adjust the X position for the logo
    int logoY = textY - 100; // Adjust the Y position for the logo

    // Draw the speedometer speed logo
    iconFont.setPixelSize(50);
    painter.setFont(iconFont);

    int iconWidth = 130; // Adjust the width of the icon as needed
    int iconHeight = 70; // Adjust the height of the icon as needed

    // Draw the current speed value beneath the logo
    QString speedValueText;
    if (status)
    {
        int speedValueX = logoX;                   // Adjust X position
        int speedValueY = logoY + iconHeight + 10; // Adjust Y position
        QRect speedValueRect(speedValueX, speedValueY, iconWidth, iconHeight);
        painter.drawText(speedValueRect, Qt::AlignCenter, QChar(0xe9e4));
        speedValueText = QString::number(speed) + " km/h";
        painter.setPen(Qt::white);                       // Reset pen color to white
        QFont boldLabelFont("Arial", 20, QFont::Normal); // Create a bold version of the label font
        painter.setFont(boldLabelFont);
        QRect speedTextRect(speedValueX, speedValueY + iconHeight, iconWidth, 30); // Adjust the height as needed
        painter.drawText(speedTextRect, Qt::AlignCenter, speedValueText);
    }
    else
    {
        int speedValueX = logoX - 5;               // Adjust X position
        int speedValueY = logoY + iconHeight + 10; // Adjust Y position
        QPen redPen(Qt::red);
        painter.setPen(redPen);
        QRect noSignalRect(speedValueX, speedValueY, iconWidth, iconHeight);
        painter.drawText(noSignalRect, Qt::AlignCenter, QChar(0xe628));
        speedValueText = "No Signal";
        QFont boldLabelFont("Arial", 20, QFont::Normal); // Create a bold version of the label font
        painter.setFont(boldLabelFont);
        QRect speedTextRect(speedValueX, speedValueY + iconHeight, iconWidth, 30); // Adjust the height as needed
        painter.drawText(speedTextRect, Qt::AlignCenter, speedValueText);
    }
}

void Canvas::drawSpeedometerNeedle(void)
{
    QPainter painter(this); // Initialize QPainter
    painter.setRenderHint(QPainter::Antialiasing, true);
    int centerX = width() / 2 * 0.9;  // Calculate the center X-coordinate of the canvas
    int centerY = height() / 2 * 1.2; // Calculate the center Y-coordinate of the canvas

    // Calculate the radius of the circle to fit within the canvas
    int radius = qMin(width(), height()) * 0.55; // Adjust the scale factor as needed

    int startA = -37 * 16; // Start angle for the markings
    int endA = 217 * 16;   // End angle for the markings

    int circleRadius = 15;

    // Draw the center circle
    QPen circlePen;
    circlePen.setColor(Qt::white);
    circlePen.setWidth(8);
    circlePen.setStyle(Qt::SolidLine);
    painter.setPen(circlePen);
    painter.setBrush(QBrush(Qt::darkRed));
    painter.drawEllipse(centerX - circleRadius, centerY - circleRadius, circleRadius * 2, circleRadius * 2);

    // Define the coordinates for the needle vertices
    qreal needleLength = radius - 45;
    qreal needleWidth = 10; // Adjust the width of the needle (increase this value)

    QVector<QPointF> needleVertices;
    needleVertices << QPointF(0, needleLength);
    needleVertices << QPointF(-needleWidth / 2, 0);
    needleVertices << QPointF(needleWidth / 2, 0);

    // Calculate the needle angle
    // qreal needleValue = 0;
    qreal needleValue = -static_cast<qreal>(speed);                                       // The value at which the needle should point (0 in this case)
    qreal zeroAngle = startA;                                                             // Angle at 0 position
    qreal needleAngle = zeroAngle + (endA - zeroAngle) * (-2.48 - (needleValue / 240.0)); // Calculate the needle angle

    painter.translate(centerX, centerY); // Translate to the center
    painter.rotate(needleAngle / 16.0);  // Rotate based on the needle angle

    // Set the color and style for the needle
    QPen needlePen;
    needlePen.setColor(Qt::darkRed); // Set the color to dark red (change this line)
    needlePen.setWidth(8);           // Set the pen width for the needle outline
    painter.setPen(needlePen);
    painter.setBrush(QBrush(Qt::darkRed)); // Set the brush color to dark red (change this line)
    painter.drawPolygon(needleVertices);   // Draw the needle polygon
}