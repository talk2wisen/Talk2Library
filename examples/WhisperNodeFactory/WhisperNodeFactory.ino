/*
 * Talk² Whisper Node AVR Factory Firmware
 * http://talk2.wisen.com.au
 *
 * This is the default program shipped with the
 * Talk² Whisper Node AVR boards.
 *
 * The program is designed to provide:
 *  - Self-testing
 *  - Remote firmware download
 *  - Basic example
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
#include "WhisperNodeFactory.h"

/* Factory Firmware */
#define T2_WP_PROGRAM_VERSION_MAJOR 1
#define T2_WP_PROGRAM_VERSION_MINOR 4

#include <RH_RF69.h>

// Stand-alone Task
void taskFunction(T2Task * Task)
{
  Serial.println(F("TaskRunning"));
  /*
   Serial.print(F("  Millis:")); Serial.println(millis());
   Serial.print(F("  LastRun:")); Serial.println(Task->lastRunTime());
   Serial.print(F("  Count:")); Serial.println(Task->runCount());
   Serial.println(F("  Stats:"));
   Serial.print(F("    Counter: ")); Serial.println(Task->statsCounter());
   Serial.print(F("    ExecTime: ")); Serial.println(Task->statsExecTime());
   */

}
T2Task myTask(&taskFunction);

// LED Example
T2Led myLedBlue(T2_WPN_LED_1);
T2Led myLedYellow(T2_WPN_LED_2);

// SPI Flash
T2Flash myFlash;

// RFM69 Radio
#define RADIO_FREQUENCY 916.0
#define RADIO_TX_POWER 13
#define RADIO_ENCRYPTION_KEY { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F }
RH_RF69 myRadio;
uint8_t radioBuf[(T2_MESSAGE_HEADERS_LEN + T2_MESSAGE_MAX_DATA_LEN)];

// T2 Message
T2Message myMsg;
#define clientAddr 0x99
#define routerAddr 0x0A
#define testAddr 0x1F

// T2 Data Buffer
T2DataBuffer myDataBuffer;

// Running Mode
enum T2BoardMode_enum: uint8_t
{
  CLIENT,
  ROUTER
};
T2BoardMode_enum runningMode = CLIENT;

// Testing
uint8_t pressedBtn_01 = 0;
uint8_t pressedBtn_02 = 0;

void setup()
{
  Serial.begin(115200);

  // Radio
  myRadio.init();
  myRadio.setModemConfig(RH_RF69::FSK_Rb125Fd125);
  myRadio.setFrequency(RADIO_FREQUENCY);
  uint8_t myRadioEncryptionKey[] = RADIO_ENCRYPTION_KEY;
  myRadio.setEncryptionKey(myRadioEncryptionKey);
  myRadio.setTxPower(RADIO_TX_POWER);

  // SPI Flash
  myFlash.init(T2_WPN_FLASH_SPI_CS);

  // Stand-alone Task
  myTask.runInterval(1000);
  myTask.runCount(2);
  myTask.status(T2TASK_STATUS_SCHEDULED);

  // LED Example
  myLedBlue.setBlink(50, 1000, -1);

  // Welcome Message
  welcomeMessage();

  // START - Test Code

  sendTestMsgs();

  // END - Test Code

}

void loop()
{
  // Stand-alone Task
  //myTask.run();

  // LED Example
  myLedBlue.run();
  myLedYellow.run();

  // START Mode
  if(runningMode == CLIENT)
  {
    runClient();

    if (Serial.available() > 0)
    {
      uint8_t serialInput = Serial.read();

      if (serialInput == 'r')
        runningMode = ROUTER;
        welcomeMessage();
        myLedYellow.setBlink(25, 500, -1);
        myLedBlue.setBlink(0, 0, 0);
        myLedBlue.turnOff();
    }

    // Test Buttons and Send voltage measurement back
    if(digitalRead(T2_WPN_BTN_1) == 1 && pressedBtn_01 == 0)
    {
      sendTestVoltage(1);
      pressedBtn_01 = 1;
    }

    if(digitalRead(T2_WPN_BTN_2) == 1 && pressedBtn_02 == 0)
    {
      sendTestVoltage(2);
      pressedBtn_02 = 1;
    }

  }

  if(runningMode == ROUTER)
  {
    runRouter();
  }
  // END Mode

}

void sendTestMsgs()
{
  uint8_t radioBufLen = 0;

  // Send Node ID on start-up as Event
  myMsg.cmd = 0x00;
  myMsg.idx = 0x06;
  myMsg.sdx = 0x01;
  myMsg.src = clientAddr;
  myMsg.dst = testAddr;
  memcpy(myMsg.data, myFlash.uniqueId, 8);
  myMsg.len = 8; //Update length
  radioBufLen = 0;
  myMsg.getSerializedMessage(radioBuf, &radioBufLen);
  myRadio.send(radioBuf, radioBufLen);
  myRadio.waitPacketSent(1000);

  delay(20);

  // Send Radio Frequency as well
  myMsg.cmd = 0x00;
  myMsg.idx = 0x06;
  myMsg.sdx = 0x6E; // RF Link
  myMsg.src = clientAddr;
  myMsg.dst = testAddr;
  myMsg.data[0] = 0x0A; // Frequency Operation
  myMsg.data[1] = (uint16_t)RADIO_FREQUENCY >> 8;     // Frequency from Float
  myMsg.data[2] = (uint8_t)(uint16_t)RADIO_FREQUENCY; // Frequency from Float
  myMsg.len = 3; //Update length
  radioBufLen = 0;
  myMsg.getSerializedMessage(radioBuf, &radioBufLen);
  myRadio.send(radioBuf, radioBufLen);
  myRadio.waitPacketSent(1000);

  delay(20);

  // Send Radio TX Power
  myMsg.cmd = 0x00;
  myMsg.idx = 0x06;
  myMsg.sdx = 0x6E; // RF Link
  myMsg.src = clientAddr;
  myMsg.dst = testAddr;
  myMsg.data[0] = 0x0B; // Frequency Operation
  myMsg.data[1] = (uint8_t)RADIO_TX_POWER;  // TX Power from int8_t
  myMsg.len = 2; //Update length
  radioBufLen = 0;
  myMsg.getSerializedMessage(radioBuf, &radioBufLen);
  myRadio.send(radioBuf, radioBufLen);
  myRadio.waitPacketSent(1000);

  delay(20);

  // Send Bootloader Version
  myMsg.cmd = 0x00;
  myMsg.idx = 0x06;
  myMsg.sdx = 0x1F; // Node Info
  myMsg.src = clientAddr;
  myMsg.dst = testAddr;
  myMsg.data[0] = 0x04; // Bootloader Version
  myMsg.data[1] = (uint8_t)(T2Utils::bootLoaderVersion() >> 8); // Major Version
  myMsg.data[2] = (uint8_t)T2Utils::bootLoaderVersion();         // Minor Version
  myMsg.len = 3; //Update length
  radioBufLen = 0;
  myMsg.getSerializedMessage(radioBuf, &radioBufLen);
  myRadio.send(radioBuf, radioBufLen);
  myRadio.waitPacketSent(1000);

  delay(20);

  // Send Firmware Version
  myMsg.cmd = 0x00;
  myMsg.idx = 0x06;
  myMsg.sdx = 0x1F; // Node Info
  myMsg.src = clientAddr;
  myMsg.dst = testAddr;
  myMsg.data[0] = 0x05; // Firmware Version
  myMsg.data[1] = (uint8_t)T2_WP_PROGRAM_VERSION_MAJOR; // Major Version
  myMsg.data[2] = (uint8_t)T2_WP_PROGRAM_VERSION_MINOR;         // Minor Version
  myMsg.len = 3; //Update length
  radioBufLen = 0;
  myMsg.getSerializedMessage(radioBuf, &radioBufLen);
  myRadio.send(radioBuf, radioBufLen);
  myRadio.waitPacketSent(1000);

}

void sendTestVoltage(uint8_t supply)
{
  uint16_t voltage = 0;

  // Send Voltage reading from supply/battery
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

  myMsg.cmd = 0x00;
  myMsg.idx = 0x06;
  myMsg.src = clientAddr;
  myMsg.dst = testAddr;
  myMsg.data[0] = 0x01; // Operation Last Reading
  myMsg.data[1] = 0x01; // Battery/Supply Index, if multiple supplies
  myMsg.data[2] = voltage >> 8;
  myMsg.data[3] = voltage;
  myMsg.len = 4; //Update length
  radioBufLen = 0;
  myMsg.getSerializedMessage(radioBuf, &radioBufLen);
  myRadio.send(radioBuf, radioBufLen);

  myRadio.waitPacketSent(1000);
}

void sendSupplyVoltages()
{
  uint8_t radioBufLen = 0;

  // Send Node ID on start-up as Event
  myMsg.cmd = 0x00;
  myMsg.idx = 0x06;
  myMsg.sdx = 0x01;
  myMsg.src = clientAddr;
  myMsg.dst = testAddr;
  memcpy(myMsg.data, myFlash.uniqueId, 8);
  myMsg.len = 8; //Update length
  radioBufLen = 0;
  myMsg.getSerializedMessage(radioBuf, &radioBufLen);
  myRadio.send(radioBuf, radioBufLen);

  myRadio.waitPacketSent(1000);
}

void welcomeMessage()
{
  // Welcome MSG
  Serial.println();
  Serial.println(F("============================================"));
  Serial.println(F(" T A L K 2   W H I S P E R   N O D E   0.3v"));
  Serial.println(F("============================================"));

  // Print This Version
  Serial.println();
  Serial.print(F("    Major Version: "));
  Serial.println(T2_WP_PROGRAM_VERSION_MAJOR, DEC);
  Serial.print(F("    Minor Version: "));
  Serial.println(T2_WP_PROGRAM_VERSION_MINOR, DEC);
  Serial.print(F("     Running Mode: "));
  if(runningMode == CLIENT) Serial.println(F("Client"));
  if(runningMode == ROUTER)  Serial.println(F("Router"));

  Serial.println();
  Serial.println(F("## Configuration ##"));

  // SPI Flash
  Serial.println();
  Serial.println(F("  :: SPI Flash ::"));
  Serial.print(F("         JEDEC ID: "));
  Serial.println(myFlash.jedecId, HEX);
  Serial.print(F("        Unique ID: "));
  for(uint8_t i = 0; i < 8; i++)
  {
    Serial.print(myFlash.uniqueId[i], HEX);
    Serial.print(':');
  }
  Serial.println();
  Serial.print(F(" Write Protection: "));
  Serial.println(myFlash.status(), BIN);

  // Radio
  Serial.println();
  Serial.println(F("  :: Radio RFM69 ::"));
  Serial.print(F("       Frequency: "));
  Serial.print(RADIO_FREQUENCY, DEC);
  Serial.println(F("MHz"));
  Serial.print(F("        TX Power: "));
  Serial.print(RADIO_TX_POWER, DEC);
  Serial.println(F("dBm"));
  Serial.print(F("  Encryption Key: "));
  uint8_t myRadioEncryptionKey[] = RADIO_ENCRYPTION_KEY;
  for(uint8_t i = 0; i < 16; i++)
  {
    Serial.print(myRadioEncryptionKey[i], HEX);
    Serial.print(':');
  }
  Serial.println();

  // Voltage
  Serial.println();
  Serial.println(F("  :: Supply Voltages ::"));
  Serial.print(F("          Battery: "));
  Serial.print(T2Utils::readVoltage(T2_WPN_VBAT_VOLTAGE, T2_WPN_VBAT_CONTROL));
  Serial.println(F("mV"));
  Serial.print(F("    Micro-USB/VIN: "));
  Serial.print(T2Utils::readVoltage(T2_WPN_VIN_VOLTAGE, T2_WPN_VIN_CONTROL));
  Serial.println(F("mV"));

  // Print Bootloader Version
  Serial.println();
  Serial.println(F("  :: Talk2 Bootloader ::"));
  Serial.print(F("    Major Version: "));
  Serial.println((uint8_t) (T2Utils::bootLoaderVersion() >> 8), DEC);
  Serial.print(F("    Minor Version: "));
  Serial.println((uint8_t) (T2Utils::bootLoaderVersion()), DEC);
  Serial.print(F("      EEPROM Data: "));
  for(uint8_t i = 0; i < T2_MCU_BOOTLOADER_EEPROM_SIZE; i++)
    {
      Serial.print(EEPROM.read(T2_MCU_BOOTLOADER_EEPROM_ADDR + i), HEX);
      Serial.print(':');
    }
    Serial.println();

  // End Welcome MSG
  Serial.println();
  Serial.println(F("============================================"));

}

void runClient()
{
  /* RFM69 Receive */
  uint8_t radioBufLen = sizeof(radioBuf);
  if(myRadio.recv(radioBuf, &radioBufLen))
  {
    myMsg.setSerializedMessage(radioBuf, radioBufLen);
    //myMsg.printMessage();

    if(myMsg.idx == 0x06 && myMsg.src == routerAddr && myMsg.dst == clientAddr)
    {
      // Update the reply Message with the new source and destination
      myMsg.dst = routerAddr;
      myMsg.src = clientAddr;
      switch(myMsg.sdx)
        {
        case 0x01: // Node Id
          T2Firmware::nodeInfo_getId();
          break;

        case 0x0A: // Power Control
          switch(myMsg.data[0])
            {
            // Restart MCU
            case 0x0A:
              T2Firmware::powerControl_restartNode();
              break;
            }
          break;

        case 0x80: // Data Buffer Operations
          switch(myMsg.data[0])
            {
            // Buffer Details
            case 0x01:
              T2Firmware::bufferOperations_bufferDetails();
              break;

            case 0x0A: // Flush To Flash
              T2Firmware::bufferOperations_flushToFlash();
              break;

              // Flush To Flash
            case 0x0B:
              T2Firmware::bufferOperations_flushToEeprom();
              break;

              // Reset Buffer
            case 0xA0:
              T2Firmware::bufferOperations_resetBuffer();
              break;
            }
          break;

        case 0x81: // Flash Operations
          switch(myMsg.data[0])
            {
            // Flash Protection (Status Register)
            case 0x90:
              T2Firmware::flashOperations_writeProtection();
              break;

            // Erase 4K
            case 0xA0:
              T2Firmware::flashOperations_erase4K();
              break;

            // Erase Chip
            case 0xAF:
              T2Firmware::flashOperations_eraseChip();
              break;
            }
          break;

        case 0x8A: // Write to Buffer
          T2Firmware::bufferOperations_bufferWrite();
          break;

        }
    }

  }
}

void runRouter()
{
  // RFM69 Receive
  uint8_t radioBufLen = sizeof(radioBuf);
  if(myRadio.recv(radioBuf, &radioBufLen))
  {

    // Print Received messages back to Serial
    for(uint8_t i = 0; i < radioBufLen; i++)
    {
      Serial.print(radioBuf[i], HEX);
      Serial.print(' ');
    }
    Serial.println();

    /*
    myMsg.setSerializedMessage(radioBuf, radioBufLen);
    myMsg.printMessage();
    */
  }

  // Execute by Serial input
  if(Serial.available() > 0)
  {

    // Serial Max Buffer, HEX data in string representation separated
    // by white spaces, followed by a LF byte.
    // Example:
    // [ 1B 80 0A C8 01 01 FF FF FF FF FF FF FF] + [LF Byte (0x0A)]
    char serialBuffer[39] = { 0 };

    // Reading Buffer
    // We expect always 38 characters
    uint8_t serialBufferLen = Serial.readBytesUntil(0x0A, serialBuffer, sizeof(serialBuffer));

    /*
    // For Debugging
    Serial.print(F("Serial Buffer Len: "));
    Serial.println(serialBufferLen, DEC);
    for(uint8_t i = 0; i < serialBufferLen; i++)
    {
      Serial.print(serialBuffer[i], HEX);
      Serial.print(':');
    }
    Serial.println();
    */

    // Need at least 14 Chars, to convert to a
    // Talk2 Message. 14 Chars representing HEX = 5 Bytes = Headers
    if (serialBufferLen >= 14)
    {
      // Converting HEX strings into byte
      char* pos = serialBuffer;
      for(uint8_t i = 0; i < sizeof(radioBuf); i++)
      {
        radioBuf[i] = strtoul(pos, &pos, 16);
      }

      // Converting to T2Message so we can discover the data length
      myMsg.setSerializedMessage(radioBuf, sizeof(radioBuf));

      /*
      // For Debugging
      for(uint8_t i = 0; i < sizeof(radioBuf); i++)
      {
        Serial.print(radioBuf[i], HEX);
        Serial.print(':');
      }
      Serial.println();
      myMsg.printMessage();
      */

      // Transmitting
      uint8_t radioBufLen = 0;
      myMsg.getSerializedMessage(radioBuf, &radioBufLen);
      myRadio.send(radioBuf, radioBufLen);
    } else {
      Serial.println(F("ERROR: Message too short."));
    }

  }
}
