/*
 * Talk2 Whisper Node AVR Firmware Upgrade Library
 * http://talk2.wisen.com.au
 *
 * This file is part of Talk2 Whisper Node AVR Firmware Upgrade Library.

 * Talk2 Whisper Node AVR Firmware Upgrade Library
 * is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Talk2 Whisper Node AVR Firmware Upgrade Library
 * is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef T2FIRMWARE_H_
#define T2FIRMWARE_H_

/* External Libraries */
#include <Arduino.h>
#include <avr/wdt.h>

/* Additional Libraries */
#include "hal/T2DataBuffer.h"
#include "hal/T2Flash.h"
#include "protocol/T2Message.h"
#include "utils/T2Utils.h"

/* SPI Flash */
extern T2Flash myFlash;

/* Radio */
#if T2_WPN_BOARD == T2_WPN_VER_RF69
#include <RH_RF69.h>
extern RH_RF69 myRadio;
#elif T2_WPN_BOARD == T2_WPN_VER_LORA
#include <RH_RF95.h>
extern RH_RF95 myRadio;
#endif
extern uint8_t radioBuf[(T2_MESSAGE_HEADERS_LEN + T2_MESSAGE_MAX_DATA_LEN)];

/* T2 Message */
extern T2Message myMsg;

/* T2 Data Buffer */
extern T2DataBuffer myDataBuffer;

class T2Firmware
{
  public:
    T2Firmware();
    virtual ~T2Firmware();


    static void nodeInfo_getId();
    static void powerControl_restartNode();
    static void bufferOperations_bufferDetails();
    static void bufferOperations_flushToFlash();
    static void bufferOperations_flushToEeprom();
    static void bufferOperations_resetBuffer();
    static void bufferOperations_bufferWrite();
    static void flashOperations_writeProtection();
    static void flashOperations_erase4K();
    static void flashOperations_eraseChip();

  private:


};

#endif /* T2FIRMWARE_H_ */
