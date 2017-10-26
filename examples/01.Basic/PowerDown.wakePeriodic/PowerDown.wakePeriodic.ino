/*
 * Talk2 Example: Power Down - Wake Periodic
 * http://talk2.wisen.com.au
 *
 * This example demonstrate how you can use the Talk2 Library to power
 * down all peripherals and using wake up every X seconds to perform a
 * task.
 * 
 * - The Sketch will use a digitalWrite to blink the LEDs
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
#include <T2WhisperNode.h>
#include <LowPower.h>

/* You need to configure the Whisper Node Version */
#define T2_WPN_BOARD T2_WPN_VER_RF69
//#define T2_WPN_BOARD T2_WPN_VER_LORA

#if T2_WPN_BOARD == T2_WPN_VER_RF69
#include <RH_RF69.h>
RH_RF69 myRadio;
#elif T2_WPN_BOARD == T2_WPN_VER_LORA
#include <RH_RF95.h>
RH_RF95 myRadio;
#endif

T2Flash myFlash;

void setup()
{
  // Serial
  Serial.begin(115200);
  Serial.println(F("Example: Power Down - Periodic"));

  Serial.println(F("Putting Radio and SPI Flash to Sleep"));
  // Radio - Initialize the radio and put it to sleep to save energy
  myRadio.init();
  myRadio.sleep();

  // Flash - We're not using, so just power it down to save energy
  myFlash.init(T2_WPN_FLASH_SPI_CS);
  myFlash.powerDown();

  // Setup the Blue LED pin
  digitalWrite(T2_WPN_LED_1, LOW); // Set LED to Off
  pinMode(T2_WPN_LED_1, OUTPUT);   // Set LED pint to OUTPUT

  // Setup the Yellow LED pin
  digitalWrite(T2_WPN_LED_2, LOW); // Set LED to Off
  pinMode(T2_WPN_LED_2, OUTPUT);   // Set LED pint to OUTPUT
}

void loop()
{
  
  // Blue LED blink means we just wake up
  digitalWrite(T2_WPN_LED_1, HIGH);
  delay(25);
  digitalWrite(T2_WPN_LED_1, LOW);

  // We old the MCU for 8 seconds so you can measure the current
  delay(8000);
  
  // Yellow LED blink means we are going to sleep
  digitalWrite(T2_WPN_LED_2, HIGH);
  delay(25);
  digitalWrite(T2_WPN_LED_2, LOW);

  // Using Low-Power library to put the MCU to Sleep
  LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF); 
}
