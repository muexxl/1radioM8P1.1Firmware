#include <Arduino.h>

extern int I2C_TX_COMPLETE;
void HAL_I2C_MasterTxCpltCallback (I2C_HandleTypeDef * hi2c)
{
  //I2C_TX_COMPLETE=0x01;
}