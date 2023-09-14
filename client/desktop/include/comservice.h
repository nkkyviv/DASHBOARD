#ifndef COMSERVICE_H
#define COMSERVICE_H

#include <mutex>
#include <atomic>
#include <cstdint>
#include "setting.h"

class COMService
{
    void extract(uint32_t start, uint32_t length, uint32_t &value);
    void extract(uint32_t start, uint32_t length, int32_t &value); // overload this function

protected:
    std::mutex mtx;
    std::atomic<bool> status{false};
    uint8_t buffer[Setting::Signal::BUFSIZE]{};

    virtual void run(void) = 0;

public:
    bool getStatus(void) { return status; }
    uint32_t getSpeed(void);
    int32_t getTemperature(void);
    uint32_t getBatteryLevel(void);
    bool getLightLeft(void);
    bool getLightRight(void);

    virtual ~COMService() = default; // virtual destructor
};

#endif