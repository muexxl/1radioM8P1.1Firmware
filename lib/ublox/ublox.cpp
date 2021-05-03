
#include <ublox.h>
#include <mxtiming.h>
#include <mxsupport.h>
// #pragma GCC push_options
// #pragma GCC optimize ("O0")

extern I2C_HandleTypeDef hi2c2;
extern int I2C_TX_COMPLETE;

const u_int8_t RTCM_SYNC{0xd3};
const u_int8_t UBLOX_REG_ADDR{0xff};

UBlox::UBlox()
{
    
}

HAL_StatusTypeDef UBlox::send(uint8_t *data, u_int8_t len)
{

    HAL_StatusTypeDef answer{HAL_ERROR};

    uint32_t now = HAL_GetTick();
    bool tx_ready = (now - last_submission) > config::UBLOX_MIN_TIME_BETWEEN_SUBMISSION_IN_MS;
    if (tx_ready)
    {

        answer = HAL_I2C_Master_Transmit_IT(&hi2c2, 0x84, &rtcm_msg[0], rtcm_msg_len);
        //answer = HAL_I2C_Master_Transmit(&hi2c2, 0x42 << 1, (uint8_t *)&data[0], len,1000);
        last_submission = now;
        flash_LED();
        //printObject(&data,len);
        //HAL_Delay(1);
        //Serial.println(len);
    }
    return answer;
}

void UBlox::add_to_RTCM_buffer(uint8_t data)
{
    rx_rtcm_buffer.put(data);
}
void UBlox::add_to_RTCM_buffer(uint8_t *data, u_int8_t len)
{
    rx_rtcm_buffer.put((uint8_t *)data, len);
}

bool UBlox::RTCM_buffer_starts_with_rtcm_sync()
{
    bool result = (0xd3u == rx_rtcm_buffer.read(0));
    return result;
}

void UBlox::clear_rtcm_buffer_until_next_sync_byte()
{
    // int bytes_dropped{0};
    while (!rx_rtcm_buffer.isEmpty())
    {
        if (RTCM_buffer_starts_with_rtcm_sync())
        {
            break;
        }
        else
        {
        rx_rtcm_buffer.get();

        }
    }
    // if (bytes_dropped)
    // {
    //     Serial.print('\n');

    //     rx_rtcm_buffer.print();
    // }
}

int UBlox::get_length_of_RTCM_msg()
{
    int length{0};
    if (rx_rtcm_buffer.get_size() < 7)
    {
        return 0;
    }
    length = ((rx_rtcm_buffer.read(1)) << 8); // first two bits of the 2nd byte
    length += rx_rtcm_buffer.read(2);         //plus second byte
    length &= 0x3ff;
    length += 6;

    return length;
}

bool UBlox::extract_message_from_RTCM_buffer()
{
    bool result{false};

    if (RTCM_buffer_starts_with_rtcm_sync())
    {
        ;
    }
    else
    {
        clear_rtcm_buffer_until_next_sync_byte();
    }

    int length = get_length_of_RTCM_msg();

    if (length == 0)
    {
        result = false;
    }
    else if (length > config::MAX_RTCM_MSG_SIZE)
    {
        rx_rtcm_buffer.get();                        //remove first byte
        clear_rtcm_buffer_until_next_sync_byte();    //clear buffer
        result = extract_message_from_RTCM_buffer(); //enter recursive loop
    }
    else if (length > rx_rtcm_buffer.get_size())
    {
        result = false;
    }
    else
    {
        rtcm_msg_len = length;
        rx_rtcm_buffer.get(&rtcm_msg[0], rtcm_msg_len);
        printObject(&rtcm_msg,rtcm_msg_len);
        bool crc_check_result = crc24q_check((unsigned char *)&rtcm_msg, rtcm_msg_len);

        if (crc_check_result)
        {
            result = true;
        }
        else
        {
            rtcm_msg_len = 0;
            result = extract_message_from_RTCM_buffer();
        }
    }
    return result;
}

void UBlox::check_rtcm_buffer_and_send_via_i2c()
{
    if (hi2c2.State != HAL_I2C_STATE_READY)
    {
        return;
    }
    HAL_StatusTypeDef send_result;

    if (rtcm_msg_len == 0)
    {
        extract_message_from_RTCM_buffer();
    }

    if (rtcm_msg_len > 0)
    {
        //try to send message
        send_result = send(&rtcm_msg[0], rtcm_msg_len);
        
        if (send_result == HAL_OK)
        {
            Serial.printf("Sent message: %d \n", rtcm_msg_len);
            rtcm_msg_len = 0;
        }
    }
}

// #pragma GCC pop_options