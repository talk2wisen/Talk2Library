/*
 * Talk2 Utility Library
 * http://talk2.wisen.com.au
 *
 *
 *  Copyright 2015-2016 by Mike Musskopf.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 */
#include "T2Utils.h"

/* Additional Libraries */
#include "T2WhisperNode.h"

T2Utils::T2Utils()
{
  //Constructor
}

T2Utils::~T2Utils()
{
  //Destructor
}

uint16_t T2Utils::bootLoaderVersion()
{
  // Returns the bootloader version
  // First byte is Major version
  // Second byte is Minor version
  return pgm_read_word(T2_MCU_BOOTLOADER_VER_ADDR);
}

uint8_t T2Utils::calcCkSum(uint8_t * data, uint8_t len)
{
  uint8_t ckSum = 0x00;
  do
  {
    ckSum += (*data++);
  } while (--len);

  return (~ckSum) + 1;
}

uint32_t T2Utils::toUint32_t(uint8_t * data, uint8_t start)
{
  return (uint32_t) data[start] << 24 | (uint32_t) data[start + 1] << 16 | (uint32_t) data[start + 2] << 8
      | (uint32_t) data[start + 3];
}


uint16_t T2Utils::readVoltage(uint8_t analogPin, uint8_t controlPin, uint16_t maxVoltage, uint8_t samples)
{
  analogReference(INTERNAL);

  // Turn on the MOSFET via control pin
  if(controlPin > 0)
  {
    digitalWrite(controlPin, HIGH);
    pinMode(controlPin, OUTPUT);
  }

  // Read pin a couple of times and keep adding up.
  uint16_t readings = 0;
  for (uint8_t i = 0; i < samples; i++) {
    readings += analogRead(analogPin);
  }

  // Turn off the MOSFET
  if(controlPin > 0)
  {
    digitalWrite(controlPin, LOW);
  }


  /* Uncomment for Debug */
  /*
  Serial.print(F("AVG last X Analog Reads on analog Pin: "));
  Serial.println(readings / samples);
  Serial.print(F("Max Voltage: "));
  Serial.println(maxVoltage);
  Serial.print(F("Calculated voltage: "));
  Serial.println((uint32_t)maxVoltage * (readings / samples) / 1023);
  */

  /*
   * This need to be casted over 32 bits to allow proper calculation.
   * Still safe to return a 16 bits for results up to 65V
  */
  return ((uint32_t)maxVoltage * (readings / samples) / 1023);
}
