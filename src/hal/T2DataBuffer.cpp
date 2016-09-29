/*
 * Talk2 Whisper Node AVR Data Buffer Library
 * http://talk2.wisen.com.au
 *
 * This file is part of Talk2 Whisper Node AVR Main Library.

 * Talk2 Whisper Node AVR Main Library
 * is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Talk2 Whisper Node AVR Main Library
 * is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 */
#include "T2DataBuffer.h"

T2DataBuffer::T2DataBuffer() :
    position(0),
    status(0x01),
    timeout(0xFF),
    buffer { 0 }
{

}

T2DataBuffer::~T2DataBuffer()
{
  // Auto-generated destructor stub
}

uint8_t T2DataBuffer::reset()
{
  this->position = 0;
  this->status = 0x01;
  this->timeout = 0xFF;
  memset(this->buffer, 0, sizeof(this->buffer));

  return this->status;
}

uint8_t T2DataBuffer::write(uint8_t * data, uint8_t len)
{
  // TODO: Implement more checks to prevent errors. At this time user need to be carefull
  if(this->status != 0x01)
  {
    // Return the Data Buffer status as error;
    return this->status;
  }

  if(len > (T2_DATABUFFER_BUFFER_SIZE - this->position))
  {
    return 0x10;
  }

  // Append data to the buffer
  memcpy(this->buffer + this->position, data, len);

  // Increment current position
  this->position += len;

  return 0x01;
}

uint8_t T2DataBuffer::flushToFlash(T2Flash * myFlash, uint8_t * crc, uint32_t addr)
{

  if(this->status == 0x02)
  {
    // Data Buffer has been already flushed
    return 0x02;
  }

  uint8_t _crc = T2Utils::calcCkSum(this->buffer, this->position);

  if( _crc == *crc)
  {
    myFlash->writePage(addr, this->buffer, this->position);
    // TODO: Maybe Re-read the Flash and re-CRC to make sure the data written is valid.

    // All Good;
    this->status = 0x02;
    return 0x01;
  }
  else
  {
    // CRC Doesn't match with Data Buffer
    *crc = _crc;
    return 0x10;
  }

}

uint8_t T2DataBuffer::flushToEeprom(uint8_t * crc, uint32_t addr)
{
  if(this->status == 0x00)
  {
    // Data Buffer is empty, nothing to flush
    return 0x02;
  }

  uint8_t _crc = T2Utils::calcCkSum(this->buffer, this->position);

  if( _crc == *crc)
  {
    for(uint8_t i = 0; i < this->position; i++)
    {
      EEPROM.write(addr + i, this->buffer[i]);
      // TODO: Maybe Re-read the EEPROM and re-CRC to make sure the data written is valid.
    }
    // All Good;
    return 0x00;
  }
  else
  {
    // CRC Doesn't match with Data Buffer
    *crc = _crc;
    return 0x10;
  }

}

void T2DataBuffer::print()
{
  Serial.println(F("------- BUFFER Dump: START -------"));
  Serial.print(F(" Pos/Len: "));
  Serial.println(this->position, DEC);
  Serial.print(F("  Status: "));
  Serial.println(this->status, DEC);
  Serial.print(F(" Timeout: "));
  Serial.println(this->timeout, DEC);
  for(uint8_t i = 0; i < this->position; i++)
  {
    Serial.print(this->buffer[i], HEX);
    Serial.print(F(":"));
  }
  Serial.println();
  Serial.print(F("     CRC: "));
  Serial.println(T2Utils::calcCkSum(this->buffer, this->position), HEX);
  Serial.println(F("-------- BUFFER Dump: END --------"));
}
