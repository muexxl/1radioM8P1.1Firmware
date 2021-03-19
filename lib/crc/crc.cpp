#include <crc.h>

uint16_t calcCRC(void * data, int len) {
	uint16_t crc = 0xffff;
	uint8_t* currentByte = reinterpret_cast<uint8_t*>(data);
	while (len--) {
        crc = crc16_update(crc, *currentByte++);
    }

	return crc;
}
uint16_t crc16_update(uint16_t crc, uint8_t a)
{
	int i;

	crc ^= a;
	for (i = 0; i < 8; ++i)
	{
		if (crc & 1)
			crc = (crc >> 1) ^ 0xA001;
		else
			crc = (crc >> 1);
	}

	return crc;
}