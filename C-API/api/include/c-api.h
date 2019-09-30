#pragma once

/**
 * \file c-api.h
 * \brief Header for the full C-API include
 *
 * Applications which link against the C-API library only have to include this
 * header file.
 *
 */

#include <init.h>
#include <stats.h>
#include <leds.h>
#include <display.h>
#include <buttons.h>
#include <leds.h>


/** \mainpage C-API - a library to abstract the SPI communication to the MCU for peripheral expansion.

 * The DRC02 and BigFish is driven by a DHCOM module and an I/O extension by a
 * MCU. The DRC02 has a Cypress PSoC MCU and the BigFish periphals are handled
 * by a Microchip PIC. The MCU is controlled by the DHCOM module through a SPI
 * interface (DHCOM is SPI Master, MCU is SPI slave) in both cases.
 *
 * Connected to MCU (PSoC/PIC):
 * \li 5 capacitive buttons
 * \li OLED Display (via SPI)
 * \li 4 LEDs
 *
 * The PSoC/PIC takes care of the control of the display, the PCAP keys query and
 * the LED control. To simplify the communication to these devices for software
 * developers a software library have been developed to offer a simple API that
 * abstracts the SPI protocol between DHCOM and the Cypress MCU. The customer
 * application is compiled against C-API and uses direct calls to API functions.
 *
 *  \image html software-structure.png
 *  \image latex software-structure.png "Software structure concerning customers application" width=10cm
 *
 * This documentation describes only the public API of this library. No
 * information regarding internal implementation is given.
 *
 * \section usage_sec Usage
 *
 * To get full access to the public API of this library only the header file
 * c-api.h has to be included.
 *
 * \section Abbreviations
 *
 * <table>
 * <caption id="multi_row">abbrevations used in this document</caption>
 * <tr><th>abb. <th>explanation
 * <tr><td>LAN<td>Local Area Network
 * <tr><td>API<td>Application Programming Interface
 * <tr><td>PCAP<td>Projected Capacitance
 * <tr><td>LED<td>Light Emitting Diode
 * <tr><td>SW<td>Software
 * <tr><td>HW<td>Hardware
 * <tr><td>HAL<td>Hardware Abstraction Layer
 * </table>
 *
 * \section reference_sec C-API Reference
 *
 * C-API methods have the following common properties:
 * \li methods return the results (error codes);
 * \li methods do not show C++ exceptions;
 * \li methods with prefix get- or set- are fast,
 *     and return immediately (non blocking),
 *     because they only return local data.
 * \li Methods with prefix write- or read- cannot return immediately (blocking)
 *     until the SPI comcommunication is terminated.
 * \li All methods are reentrant, which means that they can also be called
 *     by multiple threads.
 *
 *
 * \section Other C++ API's and Dependencies
 *
 * \subsection DHCOM_HAL
 *
 * A HAL abstraction library for DHCOM modules DHCOM_HAL is used and included.
 * If you have any questions, please contact us.
 *
 * \subsection FreeType Text API
 *
 * To draw the TTF texts we use an existing API 'FreeType' from
 * https://www.freetype.org/ in the form of the dynamic library (due to GPL
 * license). The API has all needed functions, like:
 * \li Selection of the font
 * \li Selection color of the font
 * \li Estimate the size of the border rectangle for certain text
 * \li Drawing of the glyph in the FT_Bitmaps
 *
 * To draw prepared glyphs on the bitmap the Bitmap::blit method is used.
 * Then the bitmap can be sent to the screen using C-API.
 *
 * \section compile_sec Compiling
 * It is intended to compile the C-API with CMAKE build system.
 *
 * \li clone or checkout source into local directory 'projectdir':
 *
 * Create a out of source tree build directory:
 * \li cd projectdir/..
 * \li mkdir build
 * \li cd build/
 *
 * To use the yocto toolchain you need to source the environment setup script before running cmake:
 * \li . /opt/fslc-framebuffer/2.6.2/environment-setup-armv7at2hf-neon-fslc-linux-gnueabi
 *
 * To setup the project for a Eclipse IDE do:
 * \li cmake CMakeLists.txt -G "Eclipse CDT4 - Unix Makefiles" ../psoc-host-application/
 * \li start Eclipse and use import dialog to open the directory 'build'
 * \li create a build target which just calls make
 *
 *
 * \section license_sec License
 * This library source code is distributed under terms of BSD License:
 * \n <c><b> Copyright (c) 2019, DH Electronics GmbH. All rights reserved.</b>
 * \n Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 * \li Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 * \li Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 * \li Neither the name of DH Electronics GmbH nor the names of its contributors may be used to endorse or promote
 * products derived from this software without specific prior written permission.
 * \n
 * \n <b> THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.</b>
 * </c>
 *
 **/
