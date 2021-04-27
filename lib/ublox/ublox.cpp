
#include <ublox.h>
#include <mxtiming.h>

extern I2C_HandleTypeDef hi2c2;

const u_int8_t RTCM_SYNC{0xd3};
const u_int8_t UBLOX_REG_ADDR{0xff};

UBlox::UBlox()
{
    //Wire.begin();
}

HAL_StatusTypeDef UBlox::send(char *data, u_int8_t len)
{
    HAL_StatusTypeDef answer{HAL_ERROR};
    if (hi2c2.State == HAL_I2C_STATE_READY)
    {
        answer = HAL_I2C_Master_Transmit(&hi2c2, 0x42 << 1, (uint8_t *)&UBLOX_REG_ADDR,1, 100);
        answer = HAL_I2C_Master_Transmit_IT(&hi2c2, 0x42 << 1, (uint8_t *)&data[0], len);
        flash_LED();
    }

    return answer;
}

void UBlox::add_to_RTCM_buffer(char data)
{
    rx_rtcm_buffer.put(data);
}
void UBlox::add_to_RTCM_buffer(char *data, u_int8_t len)
{
    rx_rtcm_buffer.put((char *)data, len);
}

bool UBlox::RTCM_buffer_starts_with_rtcm_sync()
{
    bool result = rx_rtcm_buffer.read(0) == RTCM_SYNC;
    return result;
}

void UBlox::clear_rtcm_buffer_until_next_sync_byte()
{

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
}

int UBlox::get_length_of_RTCM_msg()
{
    int length{0};
    if (rx_rtcm_buffer.size() < 7)
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
        rx_rtcm_buffer.get();
        result = extract_message_from_RTCM_buffer();
    }
    else if (length > rx_rtcm_buffer.size())
    {
        result = false;
    }
    else
    {
        rtcm_msg_len = length;
        rx_rtcm_buffer.get(&rtcm_msg[0], rtcm_msg_len);
        result = true;
    }
    return result;
}

void UBlox::check_rtcm_buffer_and_send_via_i2c()
{
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
            //set length to zero to avoid double sending
            rtcm_msg_len = 0;
        }
    }
}
