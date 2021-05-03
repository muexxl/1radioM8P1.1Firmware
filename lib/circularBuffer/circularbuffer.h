#if !defined(CIRCULARBUFFER_H)
#define CIRCULARBUFFER_H

#include <stdint.h>

/*

Kudos to https://embeddedartistry.com/blog/2017/05/17/creating-a-circular-buffer-in-c-and-c/


    CircularBuffer<uint32_t> circle(10);

    Adding data is easy:

    uint32_t x = 100;
    circle.put(x);

    And getting data is equally easy:

    x = circle.get()
*/
typedef enum
{
  CIRCBUF_OK       = 0x00U,
  CIRCBUF_ERROR    = 0x01U,
  CIRCBUF_OVERFLOW       = 0x02U,
  CIRCBUF_NOT_ENOUGH_DATA       = 0x03U,
} CIRCBUF_STATUS;


class CircularBuffer {
public:
	explicit CircularBuffer(int size);
    ~CircularBuffer();
	void put(uint8_t item);
	CIRCBUF_STATUS put(uint8_t * data, int len);
	uint8_t get();
	CIRCBUF_STATUS get(uint8_t * data, int len);
	void reset();
	bool isEmpty() const;
	bool isFull() const;
	bool isNotFull() const;
	int capacity() const;
	int get_size() const;
    int size;
    int available() const;
    uint8_t read(int relativePosition);
    void dropMiddleData(int len, int offset);
    int write(int relativePosition, uint8_t value);
    int findFirstMAVMessage(int * pos);
    int dropFirstMAVMessage();
    int isMAVMessage(int pos);
    void print();
    void print_sorted();
	uint8_t * buf;
    
private:
	int head = 0;
	int tail = 0;
	int maxSize; 
	bool full = 0;
};

#endif // CIRCULARBUFFER_H

/*
void test1()
{
    CircularBuffer cb(32);
    Serial.print(F("CB.get :"));
    Serial.print(cb.get());
    Serial.print(F("\n"));
    Serial.print(F(" CB.size  "));
    Serial.print(cb.size());
    Serial.print(F(" CB.capacity  "));
    Serial.print(cb.capacity());
    Serial.print(F("\n"));
    for (int i = 0; i < 40; i++)
    {
        if (!cb.isFull())
        {
            cb.put(i);
            Serial.print(F("Putting "));
            Serial.print(i);
            Serial.print(F(" CB.size  "));
            Serial.print(cb.size());
            Serial.print(F(" CB.capacity  "));
            Serial.print(cb.capacity());
            Serial.print(F("\n"));
        }
        else
        {
            Serial.print(F("Buffer is full "));
            Serial.print(F(" CB.size  "));
            Serial.print(cb.size());
            Serial.print(F(" CB.capacity  "));
            Serial.print(cb.capacity());
            Serial.print(F("\n"));
        }
    }

    for (int i = 0; i < 40; i++)
    {
        if (!cb.isEmpty())
        {
            Serial.print(cb.get());
            Serial.print(F(" CB.size  "));
            Serial.print(cb.size());
            Serial.print(F(" CB.capacity  "));
            Serial.print(cb.capacity());
            Serial.print(F("\n"));
        }
        else
        {
            Serial.print(F("Buffer is empty\n"));
        }
    }
}
*/
