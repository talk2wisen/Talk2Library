/*
 * Talk² RFM69 - Extension Library
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
#include "T2RFM69.h"

T2RFM69::T2RFM69(uint8_t cs_pin, uint8_t int_pin)
  :
  RH_RF69(cs_pin, int_pin)
{

}

T2RFM69::~T2RFM69()
{
  // TODO Auto-generated destructor stub
}


bool T2RFM69::send(T2Message *msg)
{

  /*
  uint8_t tmpData[12];
  memcpy(&tmpData, &msg->getSerializedHeaders(), 4);
  memcpy(&tmpData[4], msg->getData(), msg->getDataLen());
  */

  uint8_t i[] = { 1 };
  return RH_RF69::send(i, 1);
}

/*
bool T2RFM69::receive(T2Message *msg)
{
  if(this->available())
  {
    dataLen = 12; //Max Length: 4 bytes header + 8 bytes data
    uint8_t tmpData[dataLen];

    this->recv(tmpData, &dataLen); //Data length gets updated here

    //Load headers
    memcpy(&headers, &tmpData, 4);

    dataLen -= 4; //Remove the 4 bytes from headers
    memcpy(data, &tmpData[4], dataLen);

    return true;
  } else {
    return false;
  }

}
*/
