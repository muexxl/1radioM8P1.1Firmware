#include <sequence.h>

void Sequence::add_packet(Packet *pPacket)
{
  int packet_number{0} ;
  packet_number= pPacket->get_number();
  pPackets[packet_number] = pPacket;
}

void Packet::fill(uint8_t *msg, int len)
{
  size = len;
  u_int8_t *pBuffer = &buffer[0] ;
  while (len--)
  {
    *pBuffer++ = *msg++;
  }
}
bool Packet::is_first()
{
  return buffer[0]==0xd0;
}
bool Packet::is_last()
{
  return buffer[0]==0xdf;
}

int Packet::get_number()
{
  return buffer[0] & 0x0f;
}
