/*
 * Talk2 Protocol - Message Library
 * http://talk2.wisen.com.au
 *
 * This file is part of Talk2 Protocol - Message Library.

 * Talk2 Protocol - Message Library
 * is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Talk2 Protocol - Message Library
 * is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef T2MESSAGE_H_
#define T2MESSAGE_H_

/* External Libraries */
#include <Arduino.h>

/* Additional Libraries */

#define T2_MESSAGE_MAX_DATA_LEN 8
#define T2_MESSAGE_HEADERS_LEN 5

class T2Message
{
  public:
    T2Message();
    virtual ~T2Message();

    /* Load the data and headers from a array */
    void setSerializedMessage(uint8_t * msg, uint8_t len);

    /* Return all message, headers+data, and the full length */
    void getSerializedMessage(uint8_t * msg, uint8_t * len);

    /* Prints the whole message using Serial.print */
    void printMessage();

    uint8_t idx;
    uint8_t sdx;
    uint8_t cmd;
    uint8_t src;
    uint8_t dst;
    uint8_t rtr;
    uint8_t len;
    uint8_t data[T2_MESSAGE_MAX_DATA_LEN];

  private:


};

#endif /* T2MESSAGE_H_ */
