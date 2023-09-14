#ifndef COMSERVICE_H
#define COMSERVICE_H


#include <mutex>
#include <atomic>
#include <cstdint>
#include "setting.h"


class COMService
{
    void insert(uint32_t start, uint32_t length, uint32_t value);

protected:
    std::mutex mtx;
    std::atomic<bool> status{false};
    uint8_t buffer[Setting::Signal::BUFSIZE]{};

    virtual void run(void) = 0;

public:

    bool getstatus(void) { return status; }

    void setSpeed(uint32_t value);

    void setTemperature(int32_t value);

    void setBatteryLevel(uint32_t value);

    void setLightLeft(bool value);

    void setLightRight(bool value);

    virtual ~COMService() = default;
};

#endif