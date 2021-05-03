
#include <unity.h>
#include <Arduino.h>
#include <circularbuffer.h>
#include <mxsupport.h>

void test_true(void);
void test_false(void);
void test_char_array(void);

extern void test_circ_buff(void);
extern void test_mav_drop(void);

extern void test_mav_drop2(void);
extern void test_cb_put_get_many(void);

extern void test_ublox(void);
extern void test_ublox_rtcm_length(void);
extern void test_ublox_rtcm_extract_msg(void);

extern void test_crc(void);

extern void test_sequence(void);
extern void test_packet(void);

void setup()
{
    HAL_Delay(1000);
    UNITY_BEGIN(); // IMPORTANT LINE!
    RUN_TEST(test_true);
    RUN_TEST(test_false);
    RUN_TEST(test_char_array);

    RUN_TEST(test_circ_buff);
    RUN_TEST(test_mav_drop);
    RUN_TEST(test_mav_drop2);
    RUN_TEST(test_cb_put_get_many);

    RUN_TEST(test_ublox);
    RUN_TEST(test_ublox_rtcm_length);
    RUN_TEST(test_ublox_rtcm_extract_msg);

    RUN_TEST(test_crc);

    RUN_TEST(test_sequence);
    RUN_TEST(test_packet);

    UNITY_END();
}

void loop()
{
    ;
}

void test_true(void)
{
    TEST_ASSERT_TRUE(true);
}

void test_false(void)
{
    TEST_ASSERT_FALSE(false);
    uint8_t data[] = {0xfd, 0x10, 0x43, 0x01, 0x01, 0x10, 0x43, 0x01, 0x01, 0x6f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x71, 0x4a, 0x35, 0x75, 0x6d, 0x00, 0x00, 0x00, 0x84, 0xa4};
}

void test_char_array(void)
{
    char data[] = {"Hello World"};
    char testdata[] = {"Hello World1"};
    TEST_ASSERT_EQUAL_CHAR_ARRAY(testdata, data, 10);
}