//platformio test -v

#include <unity.h>
#include <Arduino.h>
#include <circularbuffer.h>
#include <mxsupport.h>
#include <crc.h>

const char testmsg1230[] = {0xD3, 0x00, 0x06, 0x4C, 0xE0, 0x00, 0x88, 0x10, 0x97, 0xC2, 0x44, 0x8B};
const char testmsg1087[] = {0xD3, 0x00, 0x33, 0x43, 0xF0, 0x00, 0x2E, 0xD5, 0xDA, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x69, 0x69, 0xA1, 0x2D, 0x19, 0x94, 0x17, 0x58, 0x5C, 0x85, 0xC9, 0x97, 0xB3, 0x47, 0x61, 0x45, 0xB8, 0x7E, 0xAB, 0xD0, 0x8A, 0x2A, 0x8A, 0x4E, 0x10, 0x81, 0x34, 0x70, 0xC9, 0xA0, 0x46, 0x6C, 0x9B};
const char testmsg1077[] = {0xD3, 0x00, 0x5F, 0x43, 0x50, 0x00, 0x20, 0xDD, 0x44, 0xE2, 0x00, 0x00, 0x04, 0x00, 0x80, 0x25, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x7D, 0x39, 0x35, 0x19, 0x15, 0x20, 0x00, 0x00, 0x19, 0xA0, 0xB6, 0xB7, 0xD0, 0xFF, 0xA0, 0x1B, 0x42, 0x63, 0x05, 0xFF, 0xEB, 0xAF, 0x98, 0xF5, 0xC8, 0x67, 0xB0, 0x78, 0x87, 0x7B, 0xFB, 0x8F, 0x3B, 0x4B, 0x2D, 0xA7, 0xD0, 0x58, 0xA3, 0xEB, 0x7E, 0x8C, 0x20, 0xAC, 0xEF, 0xE2, 0xB0, 0x04, 0x29, 0x19, 0xFD, 0x3E, 0x52, 0x94, 0x95, 0x29, 0x48, 0x03, 0x81, 0x48, 0x40, 0x0F, 0x83, 0xC0, 0x69, 0x7C, 0x41, 0x89, 0x42, 0x6F, 0x17, 0xB7, 0x40, 0xC0, 0xCF, 0x01, 0xD3};
const char testmsg1230_f[] = {0xD3, 0x01, 0x06, 0x4C, 0xE0, 0x00, 0x88, 0x10, 0x97, 0xC2, 0x44, 0x8B};
const char testmsg1087_f[] = {0xD3, 0x00, 0x33, 0x43, 0xF0, 0x00, 0x2E, 0xD5, 0xDA, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x69, 0x69, 0xA1, 0x2D, 0x19, 0x94, 0x17, 0x58, 0x5C, 0x85, 0xC9, 0x97, 0xB3, 0x47, 0x61, 0x45, 0xB8, 0x7E, 0xAB, 0xD0, 0x8A, 0x2A, 0x8A, 0x4E, 0x10, 0x81, 0x34, 0x70, 0xC9, 0xA0, 0x46,  0x9B,0x6C};
const char testmsg1077_f[] = {0xD3, 0x00, 0x5F, 0x43, 0x50, 0x00, 0x20, 0xDD, 0x44, 0xE2, 0x00, 0x00, 0x04, 0x00, 0x80, 0x25, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x7D, 0x39, 0x35, 0x19, 0x15, 0x20, 0x00, 0x00, 0x19, 0xA0, 0xB6, 0xB7, 0xD0, 0xFF, 0xA0, 0x1B, 0x42, 0x63, 0x05, 0xFF, 0xEB, 0xAF, 0x98, 0xF5, 0xC8, 0x67, 0xB0, 0x78, 0x87, 0x7B, 0xFB, 0x8F, 0x4B,0x3B,  0x2D, 0xA7, 0xD0, 0x58, 0xA3, 0xEB, 0x7E, 0x8C, 0x20, 0xAC, 0xEF, 0xE2, 0xB0, 0x04, 0x29, 0x19, 0xFD, 0x3E, 0x52, 0x94, 0x95, 0x29, 0x48, 0x03, 0x81, 0x48, 0x40, 0x0F, 0x83, 0xC0, 0x69, 0x7C, 0x41, 0x89, 0x42, 0x6F, 0x17, 0xB7, 0x40, 0xC0, 0xCF, 0x01, 0xD3};

void test_crc(void)
{
    bool answer;
    answer = crc24q_check((unsigned char *)&testmsg1230[0], 12);
    TEST_ASSERT_TRUE(answer);

    answer = crc24q_check((unsigned char *)&testmsg1087[0], 57);
    TEST_ASSERT_TRUE(answer);

    answer = crc24q_check((unsigned char *)&testmsg1077[0], 101);
    TEST_ASSERT_TRUE(answer);

    answer = crc24q_check((unsigned char *)&testmsg1230_f[0], 12);
    TEST_ASSERT_FALSE(answer);
    answer = crc24q_check((unsigned char *)&testmsg1087_f[0], 57);
    TEST_ASSERT_FALSE(answer);
    answer = crc24q_check((unsigned char *)&testmsg1077_f[0], 101);
    TEST_ASSERT_FALSE(answer);
}