/*
 * Talk² Example: Voltage - Node
 * http://talk2.wisen.com.au
 *
 * This example demonstrate how to send a message, in this case the voltage
 * readings from the Power Supply and the Battery, to another node.
 * 
 * - This example works together with "Voltage - Base"
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
void taskVoltageFunction(T2Task * Task); // Implemented at the bottom of this file
T2Task myTaskVoltage(&taskVoltageFunction);

void setup()
{
  // Serial
  Serial.begin(115200);
  Serial.println(F("Voltage - Node"));

  // Radio
  myRadio.init();
  myRadio.setModemConfig(RH_RF69::FSK_Rb125Fd125);
  myRadio.setFrequency(RADIO_FREQUENCY);
  uint8_t myRadioEncryptionKey[] = RADIO_ENCRYPTION_KEY;
  myRadio.setEncryptionKey(myRadioEncryptionKey);
  myRadio.setTxPower(RADIO_TX_POWER);

  // Stand-alone Task
  myTaskVoltage.runInterval(1000);   // Task will tun every 1000 milliseconds
  myTaskVoltage.runCount(0);         // Task will run forever
  myTaskVoltage.status(T2TASK_STATUS_SCHEDULED);

  // LED Example
  //myLedBlue.setBlink(20, 500, -1);   // LED will lit for 20ms every 250ms, forever
}

void loop()
{
  // Stand-alone Task - We run it as a non-blocking Task
  myTaskVoltage.run();

  // LED Example
  myLedBlue.run();
  myLedYellow.run();
}

void taskVoltageFunction(T2Task * Task)
{
  Serial.println(F("TaskRunning"));

  // Se the Yellow LED to blink, indicating are doing something
  myLedYellow.setBlink(10, 0, 1); // LED will lit for 10ms only once, so the interval doesn't matter
  
  sendTestVoltage(1);
  sendTestVoltage(2);
}

void sendTestVoltage(uint8_t supply)
{
  uint16_t voltage = 0;

  // Get Voltage readings from supply/battery
  if(supply == 1)
  {
    voltage = T2Utils::readVoltage(T2_WPN_VBAT_VOLTAGE, T2_WPN_VBAT_CONTROL);
    myMsg.sdx = 0x64; // Battery
  }

  if(supply == 2)
  {
    voltage = T2Utils::readVoltage(T2_WPN_VIN_VOLTAGE, T2_WPN_VIN_CONTROL);
    myMsg.sdx = 0x65; // Supply
  }

  uint8_t radioBufLen = 0;

  // Prepare Message
  myMsg.cmd = 0x00;
  myMsg.idx = 0x06;
  myMsg.src = nodeAddr;
  myMsg.dst = baseAddr;
  myMsg.data[0] = 0x01; // Operation Last Reading
  myMsg.data[1] = 0x01; // Battery/Supply Index, if multiple supplies
  myMsg.data[2] = voltage >> 8;
  myMsg.data[3] = voltage;
  myMsg.len = 4; //Update length
  
  // Encode Message and get the full length
  myMsg.getSerializedMessage(radioBuf, &radioBufLen);
  
  // Send it
  myRadio.send(radioBuf, radioBufLen);
  myRadio.waitPacketSent(100);
}
