/*
 * Talk² AVR Non-Blocking LED Library
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
#include "T2Led.h"

T2Led::T2Led(uint8_t pin)
  :
  _pin(pin),
  _state(false),
  _blinkPeriodOn(0),
  _blinkPeriodOff(0),
  _blinkTimes(0)
{
  digitalWrite(this->_pin, this->_state);
  pinMode(this->_pin, OUTPUT);

  this->_task = new T2Task(T2Led::staticTaskBlink, this);
}

T2Led::~T2Led()
{
  //Destructor
}

void T2Led::run()
{
  if(this->_blinkTimes != 0)
  {
    this->_task->run();
  }
}

void T2Led::setBlink(uint16_t periodOn, uint16_t periodOff, int8_t times)
{
  this->_blinkPeriodOn = periodOn;
  this->_blinkPeriodOff = periodOff;
  /* If the Led is already on, add one additional blink. But only if it's not to be infinite (-1) */
  this->_blinkTimes = (this->_state == true && times > 0) ? (times + 1) : times;

  /* Create the Task */
  this->_task->runInterval(periodOn);
  this->_task->runCount(2);
  this->_task->status(T2TASK_STATUS_SCHEDULED);
}

void T2Led::staticTaskBlink(void *cbObject, T2Task *task)
{
  ((T2Led *)cbObject)->taskBlink(task);
}

void T2Led::taskBlink(T2Task *task)
{
  if (this->_state == false)
  {
    this->turnOn();
    this->_task->runInterval(this->_blinkPeriodOn);
    this->_task->runCount(2);
    this->_task->status(T2TASK_STATUS_SCHEDULED);
  } else {
    /* If blinkTimes is negative, we will run it forever */
    if (this->_blinkTimes > 0)
    {
      this->_blinkTimes--;
    }
    this->turnOff();
    this->_task->runInterval(this->_blinkPeriodOff);
    this->_task->runCount(2);
    this->_task->status(T2TASK_STATUS_SCHEDULED);
  }
}

/* This should return true if there is no blinking activity
 * of if the Led is marked to blink forever */
bool T2Led::safeToSleep()
{
  return (this->_blinkTimes <= 0) ? true : false;
}

void T2Led::turnOn()
{
  this->_state = true;
  digitalWrite(this->_pin, this->_state);
}

void T2Led::turnOff()
{
  this->_state = false;
  digitalWrite(this->_pin, this->_state);
}

bool T2Led::getState()
{
  return this->_state;
}
