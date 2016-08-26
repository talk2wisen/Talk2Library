/*
 * Talk² AVR SPI Flash Library
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
#include "T2Flash.h"

T2Flash::T2Flash()
  :
  jedecId(0),
  uniqueId{0},
  _SPCR(0),
  _SPSR(0),
  _slaveSelectPin(0),
  _spiSettings()
{
}

T2Flash::~T2Flash()
{
  //Destructor
}


bool T2Flash::init(uint8_t cs)
{
  this->_slaveSelectPin = cs;
  digitalWrite(this->_slaveSelectPin, HIGH);
  pinMode(this->_slaveSelectPin, OUTPUT);
  SPI.begin();

  this->_spiSettings = new SPISettings(T2FLASH_CNF__SPI_SPEED, T2FLASH_CNF__SPI_ORDER, T2FLASH_CNF__SPI_MODE);

  // Read Device JEDEC ID
  this->beginTransaction();
  SPI.transfer(T2FLASH_REG__JEDEC_ID);
  this->jedecId |= (uint32_t)SPI.transfer(0) << 16;
  this->jedecId |= SPI.transfer16(0);
  this->endTransaction();

  // Read Device Unique ID
  this->beginTransaction();
  SPI.transfer(T2FLASH_REG__UNIQUE_ID);
  SPI.transfer(0);
  SPI.transfer(0);
  SPI.transfer(0);
  SPI.transfer(0);
  SPI.transfer(this->uniqueId, 8);
  this->endTransaction();

  return true;
}

bool T2Flash::beginTransaction()
{
  // Save current SPI Config
  this->_SPCR = SPCR;
  this->_SPSR = SPSR;

  SPI.beginTransaction(*this->_spiSettings);
  digitalWrite(this->_slaveSelectPin, LOW);

  return true;
}

bool T2Flash::endTransaction()
{
  digitalWrite(this->_slaveSelectPin, HIGH);
  SPI.endTransaction();

  // Restore previous SPI Config
  SPCR = this->_SPCR;
  SPSR = this->_SPSR;

  return true;
}

bool T2Flash::powerUp()
{
  this->beginTransaction();
  SPI.transfer(T2FLASH_REG__POWER_UP);
  this->endTransaction();

  return true;
}

uint8_t T2Flash::status()
{
  uint8_t status = 0x00;
  this->beginTransaction();
  SPI.transfer(T2FLASH_REG__READ_STATUS);
  status = SPI.transfer(0);
  this->endTransaction();

  return status;
}

bool T2Flash::isBusy()
{
  bool isBusy = false;
  isBusy = this->status() & 0x01; // Busy status is represented by the LSB (S0) of the Status Register
  return isBusy;
}

uint8_t T2Flash::writeStatus(uint8_t status)
{
  this->writeEnable();

  this->beginTransaction();
  SPI.transfer(T2FLASH_REG__WRITE_STATUS);
  SPI.transfer(status & T2FLASH_REG__WRITE_STATUS_MASK);
  this->endTransaction();

  return this->status();
}

bool T2Flash::writeEnable()
{
  this->beginTransaction();
  SPI.transfer(T2FLASH_REG__WRITE_ENABLE);
  this->endTransaction();

  return true;
}

bool T2Flash::writeDisable()
{
  this->beginTransaction();
  SPI.transfer(T2FLASH_REG__WRITE_DISABLE);
  this->endTransaction();

  return true;
}

uint8_t T2Flash::readByte(uint32_t addr)
{
  uint8_t data = 0x00;

  this->beginTransaction();
  SPI.transfer(T2FLASH_REG__READ_DATA);
  SPI.transfer(addr >> 16);
  SPI.transfer(addr >> 8);
  SPI.transfer(addr);
  data = SPI.transfer(0);
  this->endTransaction();

  return data;
}

bool T2Flash::writeByte(uint32_t addr, uint8_t data)
{
  this->writeEnable();

  this->beginTransaction();
  SPI.transfer(T2FLASH_REG__WRITE_DATA);
  SPI.transfer(addr >> 16);
  SPI.transfer(addr >> 8);
  SPI.transfer(addr);
  SPI.transfer(data);
  this->endTransaction();

  return true;
}

bool T2Flash::writePage(uint32_t addr, const void* data, uint16_t len)
{
  // Limit the initial looping so we don't wrap around the page
  uint16_t _maxLoopLen = T2FLASH_CNF_MAX_WRITEBYTES - (addr % T2FLASH_CNF_MAX_WRITEBYTES);
  uint16_t _offset = 0;
  uint16_t _loopLen = 0;

  while (len > 0)
  {
    // Adjust the size of looping
    if(len > _maxLoopLen)
    {
      _loopLen = _maxLoopLen;
    } else {
      _loopLen = len;
    }

    // Write the data
    this->writeEnable();

    this->beginTransaction();
    SPI.transfer(T2FLASH_REG__WRITE_DATA);
    SPI.transfer(addr >> 16);
    SPI.transfer(addr >> 8);
    SPI.transfer(addr);
    // This loop needs to be uint16_t, if not, it wraps-around when hits 256
    for (uint16_t i = 0; i < _loopLen; i++)
    {
      SPI.transfer(((uint8_t*)data)[i + _offset]);
    }
    this->endTransaction();

    // Adjust for the next interaction
    // Make sure we hit len = 0 at the end
    addr += _loopLen;
    len -= _loopLen;
    _offset += _loopLen;
    _maxLoopLen = T2FLASH_CNF_MAX_WRITEBYTES;

    // Hold until the chip is available
    // TODO: It should be very quick, but might be room to make this non-blocking
    while(this->isBusy());

  }

  return true;
}

bool T2Flash::erase4K(uint32_t addr)
{
  this->writeEnable();

  this->beginTransaction();
  SPI.transfer(T2FLASH_REG__ERASE_4K);
  SPI.transfer(addr >> 16);
  SPI.transfer(addr >> 8);
  SPI.transfer(addr);
  this->endTransaction();

  return true;
}

bool T2Flash::eraseChip()
{
  this->writeEnable();

  this->beginTransaction();
  SPI.transfer(T2FLASH_REG__ERASE_CHIP);
  this->endTransaction();

  return true;
}
