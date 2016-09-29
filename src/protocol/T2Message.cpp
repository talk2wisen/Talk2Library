/*
 * Talk2 Protocol - Message Library
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
#include "T2Message.h"

T2Message::T2Message()
  :
  idx(0),
  sdx(0),
  cmd(0),
  src(0),
  dst(0),
  rtr(0),
  len(0),
  data { 0 }
{

}

T2Message::~T2Message()
{
  // Auto-generated destructor stub
}

void T2Message::setSerializedMessage(uint8_t * msg, uint8_t len)
{

  //Extract Headers from the first 5 bytes
  this->idx = (msg[0] & 0x1C) >> 2;
  this->cmd = msg[0] & 0x03;
  this->sdx = msg[1];
  this->src = msg[2];
  this->dst = msg[3];
  this->rtr = (msg[4] & 0x80) >> 7;
  this->len = msg[4] & 0x7F;

  //Extract the Data
  memset(this->data, 0, sizeof(this->data));
  memcpy(this->data, msg + T2_MESSAGE_HEADERS_LEN, this->len);

}

void T2Message::getSerializedMessage(uint8_t * msg, uint8_t * len)
{

  // Put Headers on the first 5 Bytes
  msg[0] = (this->idx << 2) | this->cmd;
  msg[1] = this->sdx;
  msg[2] = this->src;
  msg[3] = this->dst;
  msg[4] = (this->rtr << 7) | this->len;

  // Append the Data
  memcpy(msg + T2_MESSAGE_HEADERS_LEN, this->data, this->len);

  // Return the Total Length
  *len = T2_MESSAGE_HEADERS_LEN + this->len;

}

void T2Message::printMessage()
{
  Serial.println(F("------- MSG Dump: START -------"));
  Serial.println(F("Headers: HEX [DEC]"));
  Serial.print(F("    Idx: ")); Serial.print(this->idx, HEX);Serial.print(F(" ["));Serial.print(this->idx, DEC);Serial.println(F("]"));
  Serial.print(F("    Cmd: ")); Serial.print(this->cmd, HEX);Serial.print(F(" ["));Serial.print(this->cmd, DEC);Serial.println(F("]"));
  Serial.print(F("    Sdx: ")); Serial.print(this->sdx, HEX);Serial.print(F(" ["));Serial.print(this->sdx, DEC);Serial.println(F("]"));
  Serial.print(F("    Src: ")); Serial.print(this->src, HEX);Serial.print(F(" ["));Serial.print(this->src, DEC);Serial.println(F("]"));
  Serial.print(F("    Dst: ")); Serial.print(this->dst, HEX);Serial.print(F(" ["));Serial.print(this->dst, DEC);Serial.println(F("]"));
  Serial.print(F("    Rtr: ")); Serial.print(this->rtr, HEX);Serial.print(F(" ["));Serial.print(this->rtr, DEC);Serial.println(F("]"));
  Serial.print(F(" Length: ")); Serial.println(this->len, DEC);
  for(uint8_t i = 0; i < this->len; i++)
  {
    Serial.print(this->data[i], HEX); Serial.print(F(":"));
  }
  Serial.println();
  Serial.println(F("-------- MSG Dump: END --------"));
}

