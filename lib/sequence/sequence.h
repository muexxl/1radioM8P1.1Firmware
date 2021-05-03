#ifndef SEQUENCE_H
#define SEQUENCE_H

#include <Arduino.h>
class Sequence{
  Packet * pPackets[0xa] ={nullptr};  
  void add_packet(Packet * p);
  int last_packet = 0;

}

class Packet{
    uint8_t buffer[32] {0};
    void fill(uint8_t * msg, int len);
    uint8_t size;
    bool is_first();
    bool is_last();
    int get_number();

}
#endif // !SEQUENCE_H
