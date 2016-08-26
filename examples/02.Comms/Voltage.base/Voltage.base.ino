/*
 * Talk² Example: Voltage - Base
 * http://talk2.wisen.com.au
 *
 * This example demonstrate how to receive a message and print on the Serial.
 * The message to be received is the voltage readings from the Power Supply
 * and the Battery, sent by a remote node.
 * 
 * - This example works together with "Voltage - Node"
 * - The messages are formatted using the Talk² Message frame
 * - Remember to adjust the frequency and TX Power to match your board
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
#include <RH_RF69.h>

// LED
T2Led myLedBlue(T2_WPN_LED_1);
T2Led myLedYellow(T2_WPN_LED_2);

// RFM69 Radio
#define RADIO_FREQUENCY 916.0
#define RADIO_TX_POWER 13
#define RADIO_ENCRYPTION_KEY { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F }
RH_RF69 myRadio;
uint8_t radioBuf[(T2_MESSAGE_HEADERS_LEN + T2_MESSAGE_MAX_DATA_LEN)];

// T2 Message
T2Message myMsg;
#define nodeAddr 0x88
#define baseAddr 0x0B

// T2 Data Buffer
T2DataBuffer myDataBuffer;

// Stand-alone Task
void taskPrintFunction(T2Task * Task); // Implemented at the bottom of this file
T2Task myTaskPrint(&taskPrintFunction);

// Global Variables to hold the Received data
uint16_t batVoltage = 0;
uint16_t batVoltageCounter = 0;
uint16_t supVoltage = 0;
uint16_t supVoltageCounter = 0;

void setup()
{
  // Serial
  Serial.begin(115200);
  Serial.println(F("Voltage - Base"));

  // Radio
  myRadio.init();
  myRadio.setModemConfig(RH_RF69::FSK_Rb125Fd125);
  myRadio.setFrequency(RADIO_FREQUENCY);
  uint8_t myRadioEncryptionKey[] = RADIO_ENCRYPTION_KEY;
  myRadio.setEncryptionKey(myRadioEncryptionKey);
  myRadio.setTxPower(RADIO_TX_POWER);

  // Stand-alone Task
  myTaskPrint.runInterval(1000);   // Task will tun every 1000 milliseconds
  myTaskPrint.runCount(0);         // Task will run forever
  myTaskPrint.status(T2TASK_STATUS_SCHEDULED);

  // LED Example
  myLedYellow.setBlink(20, 3000, -1);   // LED will lit for 20ms every 3000ms, forever
}

void loop()
{
  // Stand-alone Task - We run it as a non-blocking Task to print the readings
  myTaskPrint.run();

  // LED Example
  myLedBlue.run();
  myLedYellow.run();
  
  // Run the Receiving function
  runReceiver();
}

void runReceiver()
{
  /* RFM69 Receive */
  uint8_t radioBufLen = sizeof(radioBuf);
  if(myRadio.recv(radioBuf, &radioBufLen))
  {
    myMsg.setSerializedMessage(radioBuf, radioBufLen);
    // Uncomment bellow to print every received message, just be careful as
    // delays here can cause messages to be lost.
    //myMsg.printMessage();

    // We can perform some message filtering based on it's headers
    if(myMsg.idx == 0x06 && myMsg.src == nodeAddr && myMsg.dst == baseAddr)
    {

      // Lets blink something
      myLedBlue.setBlink(10, 0, 1); // LED will lit for 10ms only once, so the interval doesn't matter
      
      switch(myMsg.sdx)
      {
      case 0x64: // Battery Voltage
        // Increment Counter
        batVoltageCounter++;
        // Concatenate 2 bytes into a uint16_t variable
        batVoltage = myMsg.data[2] << 8;
        batVoltage |= myMsg.data[3];
        break;

      case 0x65: // Power Supply Voltage
        // Increment Counter
        supVoltageCounter++;
        // Concatenate 2 bytes into a uint16_t variable
        supVoltage = myMsg.data[2] << 8;
        supVoltage |= myMsg.data[3];
        break;
        
      default: // Can define an operation for everything else
        // Do something
        Serial.println(F("Unexpected message received: "));
        myMsg.printMessage();
        break;
      }
    }
  }
}

void taskPrintFunction(T2Task * Task)
{
  // We simply print the last data we got
  Serial.println(F("### Latest Voltage Readings ###"));
  Serial.print(F("       Battery: ")); Serial.print(batVoltage); Serial.print(F(" millivolts. ")); Serial.println(batVoltageCounter);
  Serial.print(F("  Power Supply: ")); Serial.print(supVoltage); Serial.print(F(" millivolts. ")); Serial.println(supVoltageCounter);
  Serial.println();
}