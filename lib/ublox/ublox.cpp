
#include <ublox.h>

extern I2C_HandleTypeDef hi2c2;

UBlox::UBlox()
{
    //Wire.begin();
}

uint8_t UBlox::send(u_int8_t *data, u_int8_t len)
{
    HAL_StatusTypeDef answer;
    answer = HAL_I2C_Master_Transmit(&hi2c2,(i2c_address<<1),data,len,100);
    //Wire.beginTransmission(i2c_address);
    //Wire.write(data,len);
    return answer ;//Wire.endTransmission(true);
}