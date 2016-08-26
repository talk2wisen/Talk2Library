/*
 * Talk² AVR Non-Blocking LED Library
 * http://talk2.wisen.com.au
 *
 * This file is part of Talk² AVR Non-Blocking LED Library.

 * Talk² AVR Non-Blocking LED Library
 * is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Talk² AVR Task Scheduler Library
 * is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef T2LED_H_
#define T2LED_H_

/* External Libraries */
#include <Arduino.h>

/* Additional Libraries */
#include "utils/T2Task.h"

class T2Led
{

  public:
    T2Led(uint8_t pin = 6); /* Defaults to D6, which is Talk2 Whisper Node LED1 */
    virtual ~T2Led();
    void setBlink(uint16_t periodOn = 50, uint16_t periodOff = 250, int8_t times = 3);
    static void staticTaskBlink(void *p, T2Task *task);

    void run();
    void turnOn();
    void turnOff();
    bool getState();

    //TODO: Implement a method like that on each peripheral
    bool safeToSleep(); /* This tells if there is any pending task */

  private:
    uint8_t _pin;
    bool _state;
    uint16_t _blinkPeriodOn;    /* Time the LED will be ON in milliseconds */
    uint16_t _blinkPeriodOff;   /* Time the LED will be OFF in milliseconds */
    int8_t _blinkTimes;         /* Number of blinks. Use -1 for infinite */

    T2Task * _task;
    void taskBlink(T2Task *task); /* This is is used by the callback method on the T2Task Class*/
};

#endif /* T2LED_H_ */
