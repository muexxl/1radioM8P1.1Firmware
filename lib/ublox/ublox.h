#ifndef UBLOX_H
#define UBLOX_H

#include <Arduino.h>
#include <circularbuffer.h>
#include <config.h>

class UBlox
{
private:
    const uint8_t i2c_address {0x42};
    CircularBuffer rx_rtcm_buffer= CircularBuffer(config::RTCM_BUFFER_SIZE);
public:
    uint8_t send(u_int8_t *data, u_int8_t len);
    void add_to_RTCM_buffer(char data);
    void add_to_RTCM_buffer(u_int8_t* data, u_int8_t len);
    UBlox();
};

#endif // UBLOXX_H