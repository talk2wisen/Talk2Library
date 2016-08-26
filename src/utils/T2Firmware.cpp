/*
 * Talk² Whisper Node AVR Firmware Upgrade Library
 * http://talk2.wisen.com.au
 *
 * This file is part of Talk² Whisper Node AVR Main Library.

 * Talk² Whisper Node AVR Main Library
 * is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Talk² Whisper Node AVR Main Library
 * is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 */
#include "T2Firmware.h"

T2Firmware::T2Firmware()
{

}

T2Firmware::~T2Firmware()
{
  // Auto-generated destructor stub
}


void T2Firmware::nodeInfo_getId()
{
  //Serial.println(F("Sending Node ID"));

  myMsg.cmd = 0x03; //Set Return
  memcpy(myMsg.data, myFlash.uniqueId, 8);
  myMsg.len = 8; //Update length

  uint8_t radioBufLen = 0;
  myMsg.getSerializedMessage(radioBuf, &radioBufLen);
  myRadio.send(radioBuf, radioBufLen);
}

void T2Firmware::powerControl_restartNode()
{
  //Serial.println(F("Restart Node"));

  wdt_enable(WDTO_500MS);
  wdt_reset();

  myMsg.cmd = 0x03; //Set Return
  myMsg.data[1] = 0x00; //Set error code
  myMsg.data[2] = 0x01; //Set error code
  myMsg.len = 3; //Update length

  uint8_t radioBufLen = 0;
  myMsg.getSerializedMessage(radioBuf, &radioBufLen);
  myRadio.send(radioBuf, radioBufLen);

  //Lock MCU to let Watch-dog kick in
  while(1)
    ;
}

void T2Firmware::bufferOperations_bufferDetails()
{
  //Serial.println(F("Sending Data Buffer Details"));

  myMsg.cmd = 0x03; //Set Return
  myMsg.data[1] = 0x00; //Set error code
  myMsg.data[2] = 0x01; //Set error code
  myMsg.data[3] = T2_DATABUFFER_BUFFER_SIZE; //Size
  myMsg.data[4] = myDataBuffer.position; //Position
  myMsg.data[5] = myDataBuffer.status; //Status
  myMsg.data[6] = myDataBuffer.timeout; //Timeout
  myMsg.len = 7; //Update length

  uint8_t radioBufLen = 0;
  myMsg.getSerializedMessage(radioBuf, &radioBufLen);
  myRadio.send(radioBuf, radioBufLen);

}

void T2Firmware::bufferOperations_flushToFlash()
{
  //Serial.println(F("Flush to Flash"));

  uint32_t addr = T2Utils::toUint32_t(myMsg.data, 2);
  uint8_t crc = myMsg.data[1];

  uint8_t result = myDataBuffer.flushToFlash(&myFlash, &crc, addr);

  myMsg.cmd = 0x03; //Set Return
  myMsg.data[1] = 0x00; //Set error code
  myMsg.data[2] = result; //Set error code
  myMsg.data[3] = crc; //CRC
  myMsg.data[4] = addr >> 24; //Addr
  myMsg.data[5] = addr >> 16; //Addr
  myMsg.data[6] = addr >  8; //Addr
  myMsg.data[7] = addr; //Addr
  myMsg.len = 8; //Update length

  /*
  //Read a bit of the Flash
  Serial.println(F("== Flash Data =="));
  Serial.print(F("Address: "));Serial.println(addr, HEX);
  for(uint8_t i = 0; i < 32; i++)
  {
    Serial.print(myFlash.readByte(addr + i), HEX);
    Serial.print(':');
  }
  Serial.println();
  Serial.println(F("== Flash Data =="));
  */

  uint8_t radioBufLen = 0;
  myMsg.getSerializedMessage(radioBuf, &radioBufLen);
  myRadio.send(radioBuf, radioBufLen);

}

void T2Firmware::bufferOperations_flushToEeprom()
{
  //Serial.println(F("Flush to EEPROM"));

  uint32_t addr = T2Utils::toUint32_t(myMsg.data, 2);
  uint8_t crc = myMsg.data[1];

  uint8_t result = myDataBuffer.flushToEeprom(&crc, addr);

  myMsg.cmd = 0x03; //Set Return
  myMsg.data[1] = 0x00; //Set error code
  myMsg.data[2] = result; //Set error code
  myMsg.data[3] = crc; //CRC
  myMsg.data[4] = addr >> 24; //Addr
  myMsg.data[5] = addr >> 16; //Addr
  myMsg.data[6] = addr >  8; //Addr
  myMsg.data[7] = addr; //Addr
  myMsg.len = 8; //Update length

  /*
  //Read a bit of the EEPROM
  Serial.println(F("== EEPROM Data =="));
  Serial.print(F("Address: "));Serial.println(addr, HEX);
  for(uint8_t i = 0; i < 32; i++)
  {
    Serial.print(EEPROM.read(addr + i), HEX);
    Serial.print(':');
  }
  Serial.println();
  Serial.println(F("== EEPROM Data =="));
  */

  uint8_t radioBufLen = 0;
  myMsg.getSerializedMessage(radioBuf, &radioBufLen);
  myRadio.send(radioBuf, radioBufLen);

}

void T2Firmware::bufferOperations_resetBuffer()
{
  //Serial.println(F("Reset Data Buffer"));

  uint8_t errorCode = myDataBuffer.reset();

  myMsg.cmd = 0x03; //Set Return
  myMsg.data[1] = 0x00; //Set error code
  myMsg.data[2] = errorCode; //Set error code
  myMsg.len = 3; //Update length

  uint8_t radioBufLen = 0;
  myMsg.getSerializedMessage(radioBuf, &radioBufLen);
  myRadio.send(radioBuf, radioBufLen);

}

void T2Firmware::bufferOperations_bufferWrite()
{
  //Serial.println(F("Write to Data Buffer"));

  uint8_t errorCode = myDataBuffer.write(myMsg.data, myMsg.len);

  myMsg.cmd = 0x03; //Set Return
  myMsg.data[0] = 0x00; //Set error code
  myMsg.data[1] = errorCode; //Set error code
  myMsg.data[2] = myDataBuffer.position; //Position
  myMsg.len = 3; //Update length

  uint8_t radioBufLen = 0;
  myMsg.getSerializedMessage(radioBuf, &radioBufLen);
  myRadio.send(radioBuf, radioBufLen);
}

void T2Firmware::flashOperations_writeProtection()
{
  //Serial.println(F("== Write Protection =="));

  if(myMsg.cmd == 0x01)
  {
    myFlash.writeStatus(myMsg.data[1]);
  }

  myMsg.cmd = 0x03; //Set Return
  myMsg.data[1] = 0x00; //Set error code
  myMsg.data[2] = 0x01; //Set error code
  myMsg.data[3] = myFlash.status(); //Return the Flash Status
  myMsg.len = 4; //Update length

  uint8_t radioBufLen = 0;
  myMsg.getSerializedMessage(radioBuf, &radioBufLen);
  myRadio.send(radioBuf, radioBufLen);
}

void T2Firmware::flashOperations_erase4K()
{
  //Serial.println(F("== Erase 4K =="));

  uint32_t addr = T2Utils::toUint32_t(myMsg.data, 1);

  myFlash.erase4K(addr);
  while(myFlash.isBusy())
  {
    //Serial.print(F("."));
    delay(1);
  }

  myMsg.cmd = 0x03; //Set Return
  myMsg.data[1] = 0x00; //Set error code
  myMsg.data[2] = 0x01; //Set error code
  myMsg.data[3] = 0x00; //Set Time Taken (Fake)
  myMsg.data[4] = 0x0A; //Set Time Taken (Fake)
  myMsg.len = 5; //Update length

  uint8_t radioBufLen = 0;
  myMsg.getSerializedMessage(radioBuf, &radioBufLen);
  myRadio.send(radioBuf, radioBufLen);
}

void T2Firmware::flashOperations_eraseChip()
{
  //Serial.println(F("== Erase Flash =="));

  myFlash.eraseChip();
  while(myFlash.isBusy())
  {
    //Serial.print(F("."));
    delay(1);
  }

  myMsg.cmd = 0x03; //Set Return
  myMsg.data[1] = 0x00; //Set error code
  myMsg.data[2] = 0x01; //Set error code
  myMsg.data[3] = 0x00; //Set Time Taken (Fake)
  myMsg.data[4] = 0x0A; //Set Time Taken (Fake)
  myMsg.len = 5; //Update length

  uint8_t radioBufLen = 0;
  myMsg.getSerializedMessage(radioBuf, &radioBufLen);
  myRadio.send(radioBuf, radioBufLen);
}
