/**********************************************************************************
*
*    Copyright (c) 2015 Richard A Burton <richardaburton@gmail.com>
*
*    This file is part of esptool2.
*
*    esptool2 is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    esptool2 is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with esptool2.  If not, see <http://www.gnu.org/licenses/>.
*
**********************************************************************************/

#ifndef CS_COMMON_PLATFORMS_ESP8266_RBOOT_ESPTOOL2_ESPTOOL2_H_
#define CS_COMMON_PLATFORMS_ESP8266_RBOOT_ESPTOOL2_ESPTOOL2_H_

#ifdef WIN32
typedef signed __int8     int8_t;
typedef signed __int16    int16_t;
typedef signed __int32    int32_t;
typedef unsigned __int8   uint8_t;
typedef unsigned __int16  uint16_t;
typedef unsigned __int32  uint32_t;
#else
#include <stdint.h>
#endif

#define true 1
#define false 0
#define bool char

void debug( const char* format, ... );
void print( const char* format, ... );
void error( const char* format, ... );

#endif /* CS_COMMON_PLATFORMS_ESP8266_RBOOT_ESPTOOL2_ESPTOOL2_H_ */
