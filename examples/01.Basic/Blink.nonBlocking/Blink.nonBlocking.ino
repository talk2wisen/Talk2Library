/*
 * Talk2 Example: Blink - Non-Blocking
 * http://talk2.wisen.com.au
 *
 * This example demonstrate how you can use the Talk2 Library to perform
 * a non-blocking LED Blink using the LED HAL and the Task Utility.
 * 
 * - The BLUE LED will run using the LED HAL
 * - The YELLOW LED will run using the Task Utility, which actually can be 
 *   used to to anything you like.
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

// Stand-alone Task - This is the "call-back" function triggered by the task. It receives the Task itself as parameter.
void taskFunction(T2Task * Task)
{
  Serial.println(F("TaskRunning"));
  digitalWrite(T2_WPN_LED_2, !digitalRead(T2_WPN_LED_2));
}
T2Task myTask(&taskFunction);

// LED Example - Create the LED instance
T2Led myLedBlue(T2_WPN_LED_1);

void setup()
{
  // Serial
  Serial.begin(115200);
  Serial.println(F("Example: Blink - Non-Blocking"));

  // Stand-alone Task to control Yellow LED
  digitalWrite(T2_WPN_LED_2, LOW); // Set LED to Off
  pinMode(T2_WPN_LED_2, OUTPUT);   // Set LED pint to OUTPUT
  myTask.runInterval(1000);        // Task will tun every 1000 milliseconds
  myTask.runCount(6);              // Task will run 6 times (3 blinks) - use "0" for infinite
  myTask.status(T2TASK_STATUS_SCHEDULED);

  // LED Example
  myLedBlue.setBlink(50, 1000, -1);   // LED will lit for 50ms every 1000ms, forever
}

void loop()
{
  // Stand-alone Task - controlling the Yellow LED
  myTask.run();

  // LED Example - Just call it and the library will take care if it needs to run or not.
  myLedBlue.run();
}