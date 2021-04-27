//platformio test -v

#include <unity.h>
#include <Arduino.h>
#include <circularbuffer.h>
#include <mxsupport.h>
#include <ublox.h>


void test_ublox(void)
{
    UBlox ub = UBlox();
    bool answer ;
    char testmsg[] = {0xD3, 0x00, 0x06, 0x4C, 0xE0, 0x00, 0x88, 0x10, 0x97, 0xC2, 0x44, 0x8B};
    
    answer = ub.RTCM_buffer_starts_with_rtcm_sync();
    TEST_ASSERT_FALSE(answer);

    ub.add_to_RTCM_buffer(&testmsg[1],10);
    answer = ub.RTCM_buffer_starts_with_rtcm_sync();
    TEST_ASSERT_FALSE(answer);
    
    ub.add_to_RTCM_buffer(&testmsg[0],12);
    answer = ub.RTCM_buffer_starts_with_rtcm_sync();
    TEST_ASSERT_FALSE(answer);


    ub.clear_rtcm_buffer_until_next_sync_byte();
    answer = ub.RTCM_buffer_starts_with_rtcm_sync();
    TEST_ASSERT_TRUE(answer);
}



void test_ublox_rtcm_length(void)
{
    UBlox ub = UBlox();
    int answer ;
    char testmsg[] = {0xD3, 0x00, 0x06, 0x4C, 0xE0, 0x00, 0x88, 0x10, 0x97, 0xC2, 0x44, 0x8B};
    char testmsg2[] = {0xD3, 0x03, 0xff, 0x4C, 0xE0, 0x00, 0x88, 0x10, 0x97, 0xC2, 0x44, 0x8B};
    char testmsg3[] = {0xD3, 0x05, 0xe6, 0x4C, 0xE0, 0x00, 0x88, 0x10, 0x97, 0xC2, 0x44, 0x8B};
    
    ub.rx_rtcm_buffer.reset();
    ub.add_to_RTCM_buffer(&testmsg[0],10);
    answer = ub.get_length_of_RTCM_msg();
    TEST_ASSERT_EQUAL(0x006,answer-6);

    ub.rx_rtcm_buffer.reset();
    ub.add_to_RTCM_buffer(&testmsg2[0],10);
    answer = ub.get_length_of_RTCM_msg();
    TEST_ASSERT_EQUAL(0x3ff,answer-6);

    ub.rx_rtcm_buffer.reset();
    ub.add_to_RTCM_buffer(&testmsg3[0],10);
    answer = ub.get_length_of_RTCM_msg();
    TEST_ASSERT_EQUAL(0x1e6,answer-6);

}


void test_ublox_rtcm_extract_msg(void)
{
    UBlox ub = UBlox();
    bool answer ;
    char testmsg[] = {0xD3, 0x00, 0x06, 0x4C, 0xE0, 0x00, 0x88, 0x10, 0x97, 0xC2, 0x44, 0x8B};
    char testmsg2[] = {0xD3, 0x03, 0xff, 0x4C, 0xE0, 0x00, 0x88, 0x10, 0x97, 0xC2, 0x44, 0x8B};
    char testmsg3[] = {0xD3, 0x05, 0xe6, 0x4C, 0xE0, 0x00, 0x88, 0x10, 0x97, 0xC2, 0x44, 0x8B};
    
    ub.add_to_RTCM_buffer(&testmsg[0],12);
    answer= ub.extract_message_from_RTCM_buffer();
    TEST_ASSERT_TRUE(answer);
    TEST_ASSERT_EQUAL_CHAR_ARRAY(testmsg,ub.rtcm_msg,12);

    ub.rtcm_msg_len=0;
    ub.rx_rtcm_buffer.reset();
    ub.add_to_RTCM_buffer(&testmsg2[0],12); //too large
    ub.add_to_RTCM_buffer(&testmsg3[0],50); //too large and some garbage
    ub.add_to_RTCM_buffer(&testmsg[0],12);
    answer= ub.extract_message_from_RTCM_buffer();

    TEST_ASSERT_TRUE(answer);
    TEST_ASSERT_EQUAL_CHAR_ARRAY(testmsg,ub.rtcm_msg,ub.rtcm_msg_len);
    



}

