/*
 * Talk2 AVR Task Scheduler Library
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
#include "T2Task.h"

T2Task::T2Task(cbFunction_t cbTask)
  :
  _lastRunTime(0),
  _runInterval(0),
  _runCount(0),
  _statsCounter(0),
  _statsExecTime(0),
  _status(T2TASK_STATUS_STOPPED),
  _cbTask(cbTask),
  _cbTaskObj(NULL),
  _cbObject(NULL)
{
}

T2Task::T2Task(cbFunctionObj_t cbTaskObj, void *cbObject)
  :
  _lastRunTime(0),
  _runInterval(0),
  _runCount(0),
  _statsCounter(0),
  _statsExecTime(0),
  _status(T2TASK_STATUS_STOPPED),
  _cbTask(NULL),
  _cbTaskObj(cbTaskObj),
  _cbObject(cbObject)
{
}

T2Task::~T2Task()
{
  //Destructor
}


void T2Task::run()
{
  if((millis() - this->_lastRunTime) >= this->_runInterval && this->_status == T2TASK_STATUS_SCHEDULED)
  {
    this->_lastRunTime = millis();
    this->_statsCounter++;
    this->_status = T2TASK_STATUS_RUNNING;
    uint32_t startMicros = micros();

    if(this->_cbObject != NULL)
    {
      this->_cbTaskObj(_cbObject, this);
    } else {
      this->_cbTask(this);
    }

    this->_statsExecTime = micros() - startMicros;

    if(this->_runCount == 0)
    {
      //This is an infinite task, reschedule it
      this->_status = T2TASK_STATUS_SCHEDULED;
    } else {
      if(--this->_runCount == 0)
      {
        //Last execution, finish it!
        this->_status = T2TASK_STATUS_FINISHED;
      } else {
        //Still more executions to go, reschedule it
        this->_status = T2TASK_STATUS_SCHEDULED;
      }
    }
  }
}

void T2Task::status(uint8_t status)
{
  this->_status = status;
}

uint8_t T2Task::status()
{
  return this->_status;
}

void T2Task::runInterval(uint32_t interval)
{
  this->_runInterval = interval;
}

uint32_t T2Task::runInterval()
{
  return this->_runInterval;
}

void T2Task::runCount(uint32_t count)
{
  this->_runCount = count;
}

uint32_t T2Task::runCount()
{
  return this->_runCount;
}

uint32_t T2Task::lastRunTime()
{
  return this->_lastRunTime;
}

uint32_t T2Task::statsCounter()
{
  return this->_statsCounter;
}

uint32_t T2Task::statsExecTime()
{
  return this->_statsExecTime;
}
