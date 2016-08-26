/*
 * Talk² Whisper Node AVR Data Buffer Library
 * http://talk2.wisen.com.au
 *
 * This file is part of Talk² Whisper Node AVR Data Buffer Library.

 * Talk² Whisper Node AVR Data Buffer Library
 * is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Talk² Whisper Node AVR Data Buffer Library
 * is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef T2DATABUFFER_H_
#define T2DATABUFFER_H_

/* External Libraries */
#include <Arduino.h>
#include <EEPROM.h>

/* Additional Libraries */
#include "T2Flash.h"
#include "utils/T2Utils.h"

#define T2_DATABUFFER_BUFFER_SIZE 32

class T2DataBuffer
{
  public:
    T2DataBuffer();
    virtual ~T2DataBuffer();

    uint8_t position;
    uint8_t status;
    uint8_t timeout;
    uint8_t buffer[T2_DATABUFFER_BUFFER_SIZE];

    uint8_t reset();
    uint8_t write(uint8_t * data, uint8_t len);
    uint8_t flushToFlash(T2Flash * myFlash, uint8_t * crc, uint32_t addr);
    uint8_t flushToEeprom(uint8_t * crc, uint32_t addr);
    void print();

  private:


};

#endif /* T2DATABUFFER_H_ */
