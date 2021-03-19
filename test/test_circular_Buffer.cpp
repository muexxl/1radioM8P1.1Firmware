//platformio test -v

#include <unity.h>
#include <Arduino.h>
#include <circularbuffer.h>
#include <mxsupport.h>
// void setUp(void) {
// // set stuff up here
// }

// void tearDown(void) {
// // clean stuff up here
// }

void test_true(void)
{
    TEST_ASSERT_TRUE(true);
}

void test_circ_buff(void)
{
    CircularBuffer cb(0x30);
    for (uint8_t i = 0; i < 0x30; i++)
    {
        cb.put(i);
    }
    TEST_ASSERT_EQUAL(0x00, cb.read(0));
    TEST_ASSERT_EQUAL(true, cb.isFull());
    TEST_ASSERT_EQUAL(0, cb.available());
    printObject(cb.buf, 0x30);

    cb.dropMiddleData(0x05, 0x10);
    printObject(cb.buf, 0x30);
    TEST_ASSERT_EQUAL(0x10, cb.available());
    TEST_ASSERT_EQUAL(1, cb.read(1));
    TEST_ASSERT_EQUAL(0, cb.read(0));
    TEST_ASSERT_EQUAL(4, cb.read(4));
    TEST_ASSERT_EQUAL(0x15, cb.read(5));
}
void test_mav_drop(void)
{
    uint8_t heartbeat[] = {0xfe,0x09,0xec,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x02,0x03,0x51,0x03,0x03,0xac,0x2d};
    uint8_t data[] = {0xfd,0x10,0x43,0x01,0x01,0x10,0x43,0x01,0x01,0x6f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x71,0x4a,0x35,0x75,0x6d,0x00,0x00,0x00,0x84,0xa4};
    uint8_t two_msgs[] = {
        0xfe, 0x19, 0x39, 0x01, 0x01, 0x16, 0x00, 0x00,
        0x00, 0x00, 0xf2, 0x03, 0xff, 0xff, 0x53, 0x54,
        0x41, 0x54, 0x5f, 0x52, 0x55, 0x4e, 0x54, 0x49,
        0x4d, 0x45, 0x00, 0x00, 0x00, 0x00, 0x06, 0x69,
        0x0a, 0xfe, 0x19, 0x3a, 0x01, 0x01, 0x16, 0x00,
        0x00, 0x00, 0x00, 0xf2, 0x03, 0xff, 0xff, 0x53,
        0x54, 0x41, 0x54, 0x5f, 0x52, 0x55, 0x4e, 0x54,
        0x49, 0x4d, 0x45, 0x00, 0x00, 0x00, 0x00, 0x06,
        0x7c, 0x6b};

    CircularBuffer cb(0xff);
    for (auto c : heartbeat)
    {
        cb.put(c);
    }
    for (auto c : data)
    {
        cb.put(c);
    }
        for (auto c : two_msgs)
    {
        cb.put(c);
    }
    printObject(cb.buf, 0xff);
    TEST_ASSERT_EQUAL(0xfe, cb.read(0));
    uint8_t len;
    uint8_t pos;
    TEST_ASSERT_GREATER_THAN_INT8(0, cb.isMAVMessage(0));
    TEST_ASSERT_EQUAL(0x09, cb.read(1));
    cb.dropFirstMAVMessage();
    TEST_ASSERT_EQUAL(0x10, cb.read(1));
    cb.dropFirstMAVMessage();
    TEST_ASSERT_EQUAL(0x19, cb.read(1));
    printObject(cb.buf, 0xff);

}

void test_mav_drop2(void)
{
    uint8_t heartbeat[] = {0xfe,0x11,0x11,0x11,0xfe,0x09,0xec,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x02,0x03,0x51,0x03,0x03,0xac,0x2d};
    uint8_t data[] = {0xfe,0x10,0x43,0x01,0x01,0x6f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x71,0x4a,0x35,0x75,0x6d,0x00,0x00,0x00,0x84,0xa4};
    uint8_t two_msgs[] = {
        0xfe, 0x19, 0x39, 0x01, 0x01, 0x16, 0x00, 0x00,
        0x00, 0x00, 0xf2, 0x03, 0xff, 0xff, 0x53, 0x54,
        0x41, 0x54, 0x5f, 0x52, 0x55, 0x4e, 0x54, 0x49,
        0x4d, 0x45, 0x00, 0x00, 0x00, 0x00, 0x06, 0x69,
        0x0a, 0xfe, 0x19, 0x3a, 0x01, 0x01, 0x16, 0x00,
        0x00, 0x00, 0x00, 0xf2, 0x03, 0xff, 0xff, 0x53,
        0x54, 0x41, 0x54, 0x5f, 0x52, 0x55, 0x4e, 0x54,
        0x49, 0x4d, 0x45, 0x00, 0x00, 0x00, 0x00, 0x06,
        0x7c, 0x6b};

    CircularBuffer cb(0xff);
    for (auto c : heartbeat)
    {
        cb.put(c);
    }
    for (auto c : data)
    {
        cb.put(c);
    }
        for (auto c : two_msgs)
    {
        cb.put(c);
    }
    cb.print();
    TEST_ASSERT_EQUAL(0xfe, cb.read(0));
    uint8_t len;
    uint8_t pos;
    TEST_ASSERT_EQUAL(0, cb.isMAVMessage(0));
    TEST_ASSERT_GREATER_THAN_INT8(0, cb.isMAVMessage(4));
    TEST_ASSERT_EQUAL(0x09, cb.read(5));
    uint16_t old_size = cb.size();
    cb.dropFirstMAVMessage();
    cb.print();
    TEST_ASSERT_EQUAL(17, old_size- cb.size());
    TEST_ASSERT_EQUAL(0x10, cb.read(5));
    cb.dropFirstMAVMessage();
    TEST_ASSERT_EQUAL(0x19, cb.read(5));
    cb.print();

}


void setup()
{

    UNITY_BEGIN(); // IMPORTANT LINE!
    RUN_TEST(test_true);
    RUN_TEST(test_circ_buff);
    RUN_TEST(test_mav_drop);
    RUN_TEST(test_mav_drop2);
    UNITY_END();
}

void loop()
{
    ;
}