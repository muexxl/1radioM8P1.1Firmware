#ifndef UBLOX_H
#define UBLOX_H

#include <Arduino.h>
#include <circularbuffer.h>
#include <config.h>
#include <crc.h>

class UBlox
{
private:
    const uint8_t i2c_address{0x42};
    uint32_t last_submission{0};
public:
    uint8_t rtcm_msg[config::MAX_RTCM_MSG_SIZE];
    int rtcm_msg_len{0};
    CircularBuffer rx_rtcm_buffer = CircularBuffer(config::RTCM_BUFFER_SIZE);
    HAL_StatusTypeDef send(uint8_t *data, u_int8_t len);
    void add_to_RTCM_buffer(uint8_t data);
    void add_to_RTCM_buffer(uint8_t *data, u_int8_t len);
    bool RTCM_buffer_starts_with_rtcm_sync();
    bool RTCM_buffer_greater_or_equal_buffer_size();
    bool next_message_starts_with_synch_byte();
    bool buffer_starts_with_message();
    bool extract_message_from_RTCM_buffer();
    bool send_message_if_available();
    int get_length_of_RTCM_msg();
    void clear_rtcm_buffer_until_next_sync_byte();
    void check_rtcm_buffer_and_send_via_i2c();


    UBlox();
};

#endif // UBLOXX_H