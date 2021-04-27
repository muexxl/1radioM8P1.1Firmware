#ifndef crc_h
#define crc_h

#include <stdint.h>

uint16_t calcCRC(void * , int ) ;
uint16_t crc16_update(uint16_t , uint8_t a);
bool crc24q_check(unsigned char *data, int len);

void crc24q_sign(unsigned char *data, int len);

unsigned crc24q_hash(unsigned char *data, int len);
#endif