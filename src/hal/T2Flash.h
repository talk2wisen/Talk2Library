/*
 * Talk² AVR SPI Flash Library
 * http://talk2.wisen.com.au
 *
 * This file is part of Talk² AVR Task Scheduler Library.

 * Talk² AVR SPI Flash Library
 * is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Talk² AVR SPI Flash Library
 * is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef T2FLASH_H_
#define T2FLASH_H_

/* External Libraries */
#include <Arduino.h>
#include <SPI.h>

/* Additional Libraries */

#define T2FLASH_CNF__SPI_SPEED       104000000
#define T2FLASH_CNF__SPI_ORDER       MSBFIRST
#define T2FLASH_CNF__SPI_MODE        SPI_MODE0

#define T2FLASH_REG__WRITE_ENABLE         0x06
#define T2FLASH_REG__WRITE_DISABLE        0x04

#define T2FLASH_REG__WRITE_STATUS         0x01
#define T2FLASH_REG__WRITE_STATUS_MASK    0xBC
#define T2FLASH_REG__BP0                  0x04
#define T2FLASH_REG__BP1                  0x08
#define T2FLASH_REG__BP2                  0x10
#define T2FLASH_REG__BTB                  0x20
#define T2FLASH_REG__READ_STATUS          0x05
#define T2FLASH_REG__READ_DATA            0x03
#define T2FLASH_REG__WRITE_DATA           0x02

#define T2FLASH_REG__JEDEC_ID             0x9F
#define T2FLASH_REG__UNIQUE_ID            0x4B

#define T2FLASH_REG__ERASE_4K             0x20
#define T2FLASH_REG__ERASE_CHIP           0x60

#define T2FLASH_REG__POWER_DOWN           0xB9
#define T2FLASH_REG__POWER_UP             0xAB

#define T2FLASH_CNF_MAX_WRITEBYTES        256

class T2Flash
{

  public:
    T2Flash();
    virtual ~T2Flash();

    bool init(uint8_t cs);
    bool powerUp();
    bool powerDown();

    bool isBusy();
    uint8_t status();
    uint8_t writeStatus(uint8_t status);

    uint8_t readByte(uint32_t addr);

    bool writeByte(uint32_t addr, uint8_t data);
    bool writePage(uint32_t addr, const void* data, uint16_t len);

    bool erase4K(uint32_t addr);
    bool eraseChip();

    uint32_t jedecId;
    uint8_t uniqueId[8];

  private:
    uint8_t _SPCR;
    uint8_t _SPSR;
    uint8_t _slaveSelectPin;

    SPISettings * _spiSettings;

    bool beginTransaction();
    bool endTransaction();

    bool writeEnable();
    bool writeDisable();

};

#endif /* T2FLASH_H_ */
