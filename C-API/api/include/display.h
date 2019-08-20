#pragma once

/**
 * \file display.h
 * \brief Header for API to control the OLED Display
 *
 * The OLED display is connected to the MCU with SPI.
 */

#include "resultcodes.h"


struct Bitmap;
struct FT_Bitmap_;

/**
 * \brief enable/disable the display
 *
 * Switch the display enable output of the MCU.
 *
 * \attention This is only available in
 * BigFish hardware with PIC. It does set the OLED#Reset line (low
 * active). DRC02 Hardware with PSoC does return OK to be legacy compatible but
 * without any action.
 *
 * \param on true == enabled / false == disabled
 *
 * \return result code of requested operation
 */
enum RESULT displayEnable(int on);

/**
 * \brief set display contrast
 *
 * \attention This feature is blocked by latest MCU firmwares.
 * The contrast values are handled by the MCU firmware and can not be adjusted.
 * See displaySetDimTimeout() and displaySetOffTimeout().
 *
 * \param value new display contrast value
 *
 * \return result code of requested operation
 */
enum RESULT displaySetContrast(int value);

/**
 * \brief set display dimming timeout
 *
 * After this time the brightness of the OLED display is reduced. The timeout
 * is reseted or/and the display reactivated/brightness increased by touching
 * the PCAP buttons. This serves to protect the OLED display.
 *
 * \param value new display dimming timeout [sec]
 *
 * \return result code of requested operation
 */
enum RESULT displaySetDimTimeout(short value);

/**
 * \brief set display switch off timeout
 *
 * This timeout starts after the dimming timeout expired and the display
 * brightness was decreased. The switch off timeout does disable the display.
 * The timeout is reseted or/and the display reactivated/brightness increased
 * by touching the PCAP buttons. This serves to protect the OLED display.
 *
 * \param value new display switch off timeout [sec]
 *
 * \return result code of requested operation
 */
enum RESULT displaySetOffTimeout(short value);

/**
 * \brief fill display with single color
 *
 * \param white if true than fill display with white (foreground color)
 *
 * \return result code of requested operation
 */
enum RESULT displayFill(bool white = false);

/**
 * \brief draw a filled rectangle
 *
 * \param x coordinate [pixel]
 * \param y coordinate [pixel]
 * \param w width [pixel]
 * \param h height [pixel]
 * \param white foreground (true) or background color (false)
 *
 * \return result code of requested operation
 */
enum RESULT displayFillRect(int x, int y, int w, int h, bool white = true);

/**
 * \brief draw a rectangle
 *
 * \param x coordinate [pixel]
 * \param y coordinate [pixel]
 * \param w width [pixel]
 * \param h height [pixel]
 * \param white foreground (true) or background color (false)
 *
 * \return result code of requested operation
 */
enum RESULT displayDrawRect(int x, int y, int w, int h, bool white = true);

/**
 * \brief invert pixels in rectangle
 *
 * \param x coordinate [pixel]
 * \param y coordinate [pixel]
 * \param w width [pixel]
 * \param h height [pixel]
 *
 * \return result code of requested operation
 */
enum RESULT displayInvertRect(int x, int y, int w, int h);

/**
 * \brief write bitmap
 *
 * \param x coordinate [pixel]
 * \param y coordinate [pixel]
 * \param bmp pointer to pixmap/bitmap
 *
 * \return result code of requested operation
 */
enum RESULT displayBitmap(int x, int y, const Bitmap &bmp);

/**
 * \brief write bitmap
 *
 * \param x coordinate [pixel]
 * \param y coordinate [pixel]
 * \param bmp pointer to pixmap/bitmap
 *
 * \return result code of requested operation
 */
enum RESULT displayBitmap(int x, int y, const struct FT_Bitmap_ &bmp);

/**
 * \brief write display frame to OLED display via MCU
 *
 * Transmit the current frame to the OLED display.
 *
 * \return result code of requested operation
 */
enum RESULT displayFlush();

/**
 * \brief re-send the last compressed buffer
 *
 * \return result code of requested operation
 */
enum RESULT displaySwap();

/**
 * \brief write splash
 *
 * Write the current compressed buffer into MCU flash
 *
 * \return result code of requested operation
 */
enum RESULT displayWriteSplash();
