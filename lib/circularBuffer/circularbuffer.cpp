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

int CircularBuffer::size() const
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
	return maxSize - size();
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

uint8_t CircularBuffer::read(uint8_t relativePosition) const
{
	uint8_t val{0};
	if (relativePosition > size())
	{
		return 0;
	}
	uint8_t realPosition = (tail + relativePosition) % maxSize;
	val = buf[realPosition];
	return val;
}

int CircularBuffer::write(uint8_t relativePosition, uint8_t value)
{
	if (relativePosition > size())
	{
		return 0;
	}
	uint8_t realPosition = (tail + relativePosition) % maxSize;
	buf[realPosition] = value;

	return 1;
}

void CircularBuffer::dropMiddleData(uint8_t len_to_preserve, uint8_t len_middle_data)
{
	while (len_to_preserve--)
	{
		write(len_to_preserve + len_middle_data, read(len_to_preserve));
	}
	full = false;
	tail = (tail + len_middle_data) % maxSize;
}

uint8_t CircularBuffer::dropFirstMAVMessage() // TODO HIER MUSSS EIN FEHLER SEIN// NOCHMAL TESTEN !!
{
	uint8_t pos;
	uint8_t len = findFirstMAVMessage(&pos);

	if (len)
	{
		dropMiddleData(pos, len);
	}
	return len;
}

uint8_t CircularBuffer::findFirstMAVMessage(uint8_t *pos)
{
	uint8_t len{0};
	for (int i = 0; i < size(); i++)
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

	if (pos + message_length >= size())
	{
		int max_length{size() - pos};
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

void CircularBuffer::print(){
	char tmp[size()];
	for (size_t i = 0; i < size(); i++)
	{
		tmp[i] = read(i);
	}
	printObject(&tmp, size());
}