/*
 * Talk2 RFM69 - Extension Library
 * http://talk2.wisen.com.au
 *
 * This file is part of Talk2 RFM69 - Extension Library

 * Talk2 RFM69 - Extension Library
 * is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Talk2 RFM69 - Extension Library
 * is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef T2RFM69_H_
#define T2RFM69_H_

#include <Arduino.h>
#include <RH_RF69.h>
#include "protocol/T2Message.h"

class T2RFM69: public RH_RF69
{
  public:
    T2RFM69(uint8_t cs_pin = 10, uint8_t int_pin = 2);
    ~T2RFM69();
    bool send(T2Message *msg);
    bool receive(T2Message *msg);
};

#endif /* T2RFM69_H_ */
