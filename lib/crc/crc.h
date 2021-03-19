#ifndef crc_h
#define crc_h

#include <stdint.h>

uint16_t calcCRC(void * , int ) ;
uint16_t crc16_update(uint16_t , uint8_t a);

#endif