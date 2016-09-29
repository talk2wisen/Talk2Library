/*
 * Talk2 Utility Library
 * http://talk2.wisen.com.au
 *
 * This file is part of Talk2 Utility Library

 * Talk2 Utility Library
 * is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Talk2 Utility Library
 * is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef T2UTILS_H_
#define T2UTILS_H_

/* External Libraries */
#include <Arduino.h>

/* Additional Libraries */

class T2Utils
{

  public:
    T2Utils();
    virtual ~T2Utils();

    static uint16_t bootLoaderVersion();
    static uint8_t calcCkSum(uint8_t * data, uint8_t len);
    static uint32_t toUint32_t(uint8_t * data, uint8_t start);
    static uint16_t readVoltage(uint8_t analogPin, uint8_t controlPin = 0, uint16_t maxVoltage = 7282, uint8_t samples = 5);

  private:

};

#endif /* T2UTILS_H_ */
