#pragma once
#pragma GCC visibility push(default)

/**
 * \file display.h
 * \brief Header for API to control the OLED Display
 *
 * The OLED display is connected to the MCU with SPI.
 *
 * To avoid wearing of the OLED display, it is dimmed after a first timeout and
 * the output is deactivated after another timeout. The OLED display can only be
 * reactivated by touching a PCAP button on the front of the device. Touching a
 * PCAP button also resets the timeouts.
 * See displaySetDimTimeout() and displaySetOffTimeout().
 */

#include "resultcodes.h"
#include <stdbool.h>


#ifdef __cplusplus
extern "C" {
#endif


/** \def DISPLAY_DEFAULT_WIDTH
    \brief display resolution in x-direction

    Resolution of the OLED SSD1306 in x-direction. Used in displayScreen().
*/
#define DISPLAY_DEFAULT_WIDTH 128

/** \def DISPLAY_DEFAULT_HEIGHT
    \brief display resolution in y-direction

    Resolution of the OLED SSD1306 in y-direction. Used in displayScreen().
*/
#define DISPLAY_DEFAULT_HEIGHT 64

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
 * See displaySetDimTimeout(), displaySetOffTimeout() and displaySetOLedSaver()
 * for further details.
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
 * the PCAP buttons. This serves to protect the OLED display. The default
 * value is 300 seconds. The maximum value is 300 seconds.
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
 * by touching the PCAP buttons. This serves to protect the OLED display. The
 * default value is 600 seconds. The maximum value is 600 seconds.
 *
 * \param value new display switch off timeout [sec]
 *
 * \return result code of requested operation
 */
enum RESULT displaySetOffTimeout(short value);

#ifndef DOXYGEN_SHOULD_SKIP_THIS
/**
 * \brief enable or disable display OLed-Saver by MCU
 *
 * To protect the OLED display, the DimTimeout and OffTimeout timers are used
 * to dim and then deactivate the display. This is done by the MCU on the
 * display unit. The only way to reset the timeouts is by touching the
 * PCAP buttons. This is the standard behavior and the easiest solution
 * to ensure a long lifetime of the OLED display.
 * To allow the use of a specific screen saver this can be disabled with this
 * API call. The timers DimTimeout and OffTimeout are then without function 
 * and it is again possible to set the display brightness with displaySetContrast().
 *
 * \param enable true == enabled (default) / false == disabled
 *
 * \return result code of requested operation
 */
enum RESULT displaySetOLedSaver(int enable);
#endif  /* DOXYGEN_SHOULD_SKIP_THIS */

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
enum RESULT displayBitmap(int x, int y, const Bitmap *bmp);

/**
 * \brief write bitmap
 *
 * \param x coordinate [pixel]
 * \param y coordinate [pixel]
 * \param bmp pointer to pixmap/bitmap
 *
 * \return result code of requested operation
 */
enum RESULT displayBitmap2(int x, int y, const struct FT_Bitmap_ *bmp);

/**
 * \brief write buffer content to (full) screen
 *
 * \param screen_buffer pointer to buffer with content to display
 *
 * For the resolution of the OLED SSD1306, the transferred
 * buffer must contain data for a full screen. See DISPLAY_DEFAULT_WIDTH and
 * DISPLAY_DEFAULT_HEIGHT. For each pixel there must be at least one bit in the
 * buffer.
 *
 * \return result code of requested operation
 */
enum RESULT displayScreen(const unsigned char *screen_buffer);

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

/**
 * \brief take BMP screenshot
 *
 * Take screenshot of OLED and write to BMP file
 *
 * \return result code of requested operation
 */
enum RESULT displayTakeScreenshot();


#ifdef __cplusplus
}
#endif
#pragma GCC visibility pop

