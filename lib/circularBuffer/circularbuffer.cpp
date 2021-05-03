#include <circularbuffer.h>
#include <mxsupport.h>
#include <Arduino.h>

CircularBuffer::CircularBuffer(int size)
{

	buf = new uint8_t[size];
	maxSize = size;
}

CircularBuffer::~CircularBuffer()
{
	delete[] buf;
}

void CircularBuffer::reset()
{
	head = tail;
	full = false;
}

bool CircularBuffer::isEmpty() const
{
	//if head and tail are equal, we are empty
	return (!full && (head == tail));
}

bool CircularBuffer::isFull() const
{
	//If tail is ahead the head by 1, we are full
	return full;
}

bool CircularBuffer::isNotFull() const
{
	//If tail is ahead the head by 1, we are full
	return !full;
}

int CircularBuffer::capacity() const
{
	return maxSize;
}

int CircularBuffer::get_size() const
{
	// returns current size
	int size = maxSize;
	if (!full)
	{
		if (head >= tail)
		{
			size = head - tail;
		}
		else
		{
			size = maxSize + head - tail;
		}
	}

	return size;
}
int CircularBuffer::available() const
{
	return maxSize - get_size();
}

void CircularBuffer::put(uint8_t item)
{
	buf[head] = item;

	if (full)
	{
		tail = (++tail) % maxSize;
	}

	head = (++head) % maxSize;

	full = tail == head;
	size = get_size();
}

CIRCBUF_STATUS CircularBuffer::put(unsigned char *data, int len)
{

	CIRCBUF_STATUS result;
	if (len <= available())
	{
		result = CIRCBUF_OK;
	}
	else
	{
		result = CIRCBUF_OVERFLOW;
	}

	while (len--)
	{
		put(*data++);
	}

	return result;
}

CIRCBUF_STATUS CircularBuffer::get(uint8_t *data, int len)
{
	CIRCBUF_STATUS answer;

	if (len <= get_size())
	{
		answer = CIRCBUF_OK;
	}
	else
	{

		answer = CIRCBUF_NOT_ENOUGH_DATA;
	}

	while (len--)
	{
		*data++ = get();
	}
	return answer;
}

uint8_t CircularBuffer::get()
{
	uint8_t val{0};
	if (isEmpty())
	{
		return 0;
	}

	//Read data and advance the tail (we now have a free space)
	val = buf[tail];
	full = false;
	tail = (tail + 1) % maxSize;

	return val;
}

uint8_t CircularBuffer::read(int relativePosition)
{
	uint8_t val{0};
	if (relativePosition > get_size())
	{
		return 0;
	}
	int realPosition = (tail + relativePosition) % maxSize;
	val = buf[realPosition];
	return val;
}

int CircularBuffer::write(int relativePosition, uint8_t value)
{
	if (relativePosition > get_size())
	{
		return 0;
	}
	int realPosition = (tail + relativePosition) % maxSize;
	buf[realPosition] = value;

	return 1;
}

void CircularBuffer::dropMiddleData(int len_to_preserve, int len_middle_data)
{
	while (len_to_preserve--)
	{
		write(len_to_preserve + len_middle_data, read(len_to_preserve));
	}
	full = false;
	tail = (tail + len_middle_data) % maxSize;
}

int CircularBuffer::dropFirstMAVMessage() // TODO HIER MUSSS EIN FEHLER SEIN// NOCHMAL TESTEN !!
{
	int pos;
	int len = findFirstMAVMessage(&pos);

	if (len)
	{
		dropMiddleData(pos, len);
	}
	return len;
}

int CircularBuffer::findFirstMAVMessage(int *pos)
{
	int len{0};
	for (int i = 0; i < get_size(); i++)
	{
		len = isMAVMessage(i);
		if (len)
		{
			*pos = i;
			break;
		}
	}
	return len;
}

int CircularBuffer::isMAVMessage(int pos)
{
	const uint8_t magic_v1{0xfe};
	const uint8_t header_len_v1{6};

	const uint8_t magic_v2{0xfd};
	const uint8_t header_len_v2{10};

	uint8_t value = read(pos);
	uint8_t message_length;
	if (value == magic_v1)
	{
		message_length = read(pos + 1) + header_len_v1 + 2;
	}
	else if (value == magic_v2)
	{
		message_length = read(pos + 1) + header_len_v2 + 2; //payload length + header length + 2 byte CRC + signature ?:/
	}
	else
	{
		return 0;
	}

	if (pos + message_length >= get_size())
	{
		int max_length{get_size() - pos};
		return max_length;
	}
	else
	{
		value = read(pos + message_length);

		if (value == magic_v1 || value == magic_v2)
		{

			return message_length;
		}
		else
		{
			return 0;
		}
	}
	return 0;
}

void CircularBuffer::print()
{
	char tmp[get_size()];
	for (size_t i = 0; i < get_size(); i++)
	{
		tmp[i] = read(i);
	}
	printObject(&tmp, get_size());
}

void CircularBuffer::print_sorted()
{

	for (size_t i = 0; i < get_size(); i++)
	{
		/* code */

		//cout << std::dec << counter++ << ":";

		if (i % 0x10 == 0)
		{
			if (i)
			{
				Serial.print(F("\n"));
			}

			if (i < 0x10)
			{
				Serial.print(0);
			}
			Serial.print(i, HEX);
			Serial.print(F("  |  "));
		}

		if (int(read(i)) < 0x10)
		{
			Serial.print(0);
		}
		Serial.print(int(read(i)), HEX);
		Serial.print(" ");
	}
	Serial.print("\n");
}