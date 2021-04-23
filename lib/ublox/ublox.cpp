
#include <ublox.h>

extern I2C_HandleTypeDef hi2c2;

UBlox::UBlox()
{
    //Wire.begin();
}

uint8_t UBlox::send(u_int8_t *data, u_int8_t len)
{
    HAL_StatusTypeDef answer{HAL_ERROR};
    if (hi2c2.State == HAL_I2C_STATE_READY ){
        answer = HAL_I2C_Master_Transmit_IT(&hi2c2,0x42<<1,&data[0],len);
    }
    
    return answer ;//Wire.endTransmission(true);
}