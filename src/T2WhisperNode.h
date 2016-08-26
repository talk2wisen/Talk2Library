/*
 * Talk² Whisper Node AVR Main Library
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

#ifndef T2WHISPERNODE_H_
#define T2WHISPERNODE_H_

/* MCU */
#define T2_MCU_BOOTLOADER_VER_ADDR     0x7FFE // Internal Memory Address where we store the bootloader version
#define T2_MCU_BOOTLOADER_EEPROM_ADDR  0x03E0 // EEPROM Memory Address where the Bootloader configuration is located
#define T2_MCU_BOOTLOADER_EEPROM_SIZE  32     // EEPROM Bootloader configuration size

/* Buttons and LEDs Pins and Ports */
#define T2_WPN_BTN_1            4     // D4
#define T2_WPN_BTN_2            5     // D5
#define T2_WPN_LED_1            6     // D6 - Blue
#define T2_WPN_LED_2            9     // D9 - Yellow

/* Voltage Dividers Pins and Ports */
#define T2_WPN_VBAT_VOLTAGE     A6    // Need to turn the CONTROL pin HIGH before read Analog 6
#define T2_WPN_VBAT_CONTROL     A0    // P-Mosfet to connect/disconnect the Battery Voltage Divider
#define T2_WPN_VBAT_MAX         7282  // Max millivolts when using RTop 562K and RBot 100K resistors @ 1.1V (internal) as VRef
#define T2_WPN_VIN_VOLTAGE      A7    // Always connected to the Analog 7
#define T2_WPN_VIN_CONTROL      0     // Not Used
#define T2_WPN_VIN_REFERENCE    7282  // Max millivolts when using RTop 562K and RBot 100K resistors @ 1.1V (internal) as VRef

/* Flash */
#define T2_WPN_FLASH_SPI_CS     8     // Chip Select
#define T2_WPN_FLASH_SPI_MOSI   11
#define T2_WPN_FLASH_SPI_MISO   12
#define T2_WPN_FLASH_SPI_SCK    13

/* Radio Module */
#define T2_WPN_RADIO_INT        2     // Hardware Interrupt
#define T2_WPN_RADIO_RESET      7     // Hardware Reset, pulled LOW by default. Turn it HIGH for a 1ms and release. Wait 5ms to reconfigure the radio
#define T2_WPN_RADIO_SPI_CS     8     // Chip Select
#define T2_WPN_RADIO_SPI_MOSI   11
#define T2_WPN_RADIO_SPI_MISO   12
#define T2_WPN_RADIO_SPI_SCK    13

/* RTC */
//TODO: Need to complete this part

/* External Libraries */
#include <Arduino.h>
#include <avr/pgmspace.h>
#include <SPI.h>

/* Additional Libraries */
#include <hal/T2Led.h>
#include <hal/T2Flash.h>
#include <hal/T2DataBuffer.h>
#include <protocol/T2Message.h>
#include <utils/T2Task.h>
#include <utils/T2Firmware.h>

class T2WhisperNode
{

  public:
    T2WhisperNode();
    virtual ~T2WhisperNode();

  private:

};

#endif /* T2WHISPERNODE_H_ */
