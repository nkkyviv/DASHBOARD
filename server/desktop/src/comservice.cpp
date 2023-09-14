#include <cstddef>
#include <climits>
#include "comservice.h"

void COMService::insert(uint32_t start, uint32_t length, uint32_t value)
{
    int pos = start % CHAR_BIT; // calculate the byte and bit indices where the insertion should start. pos represents the bit position within a byte, and index represents the index of the byte in the buffer.
    int index = start / CHAR_BIT;
    std::scoped_lock<std::mutex> lock(mtx);

    for (uint32_t i = 0; i < length; i++)
    {
        uint8_t bit = (value >> i) & 1; // Extracts the i-th bit from the value by right-shifting the value by i positions and performing a bitwise AND with 1
        buffer[index] &= ~(1 << pos);   // Clears the bit at position pos in the buffer[index] byte.
        buffer[index] |= (bit << pos);  // Sets the bit at position pos in the buffer[index] byte to the value of the bit extracted from the value

        pos++;
        if (pos == CHAR_BIT)
        {
            pos = 0;
            index++; // update the pos and index values to move to the next bit position within the byte or the next byte in the buffer when the current byte is filled.
        }
    }
}

void COMService::setSpeed(uint32_t value)
{
    insert(Setting::Signal::Speed::START, Setting::Signal::Speed::LENGTH, value);
}

void COMService::setTemperature(int32_t value)
{
    insert(Setting::Signal::Temperature::START, Setting::Signal::Temperature::LENGTH, value);
}

void COMService::setBatteryLevel(uint32_t value)
{
    insert(Setting::Signal::BatteryLevel::START, Setting::Signal::BatteryLevel::LENGTH, value);
}

void COMService::setLightLeft(bool value)
{
    uint32_t lightValue = value ? 1 : 0; // It converts the boolean value to a corresponding unsigned integer value (1 for true and 0 for false).
    insert(Setting::Signal::Light::Left::START, Setting::Signal::Light::Left::LENGTH, lightValue);
}

void COMService::setLightRight(bool value)
{
    uint32_t lightValue = value ? 1 : 0; // It converts the boolean value to a corresponding unsigned integer value (1 for true and 0 for false).
    insert(Setting::Signal::Light::Right::START, Setting::Signal::Light::Right::LENGTH, lightValue);
}
