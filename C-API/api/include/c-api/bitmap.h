#pragma once

/**
 * \file bitmap.h
 * \brief Header for Bitmap definitions
 *
 */

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \struct Bitmap
 * \brief Structure for handling image information.
 *
 * A structure used to describe a bitmap or pixmap
 *
 * \attention The bitmap must contain one bit per pixel and the bits must be
 * arranged in lines. Line breaks are made on byte boundaries (pitch bits per
 * line), although the width of the visible bitmap does not always fall exactly
 * to the byte boundary (width bits). The remaining (pitch) bits at the end of
 * the line are ignored.
 *
 *  \image html bitmap.png
 *  \image latex bitmap.png "Bitmap data arrangement" width=16cm
 *
 */
struct Bitmap
{
    Bitmap(int w, int h, const unsigned char *bytes = NULL);
    Bitmap(const Bitmap &other);
    ~Bitmap();

    const int width;     ///< visible width [pixel]
    const int height;	 ///< visible height [lines/pixel]
    const int pitch;	 ///< The pitch’s value is the number of bytes taken by one bitmap row, including padding. [pixel]
    const int size;		 ///< size of data buffer [bytes]
    unsigned char *data; ///< data buffer
};

/**
 * \struct FT_Bitmap_
 * \brief Bitmap based on FreeType API
 *
 * A structure used to describe a bitmap or pixmap. See
 * \li https://www.freetype.org/freetype2/docs/reference/ft2-bitmap_handling.html
 * and
 * \li https://www.freetype.org/freetype2/docs/reference/ft2-basic_types.html#ft_bitmap
 *
 * for further information.
 */
struct  FT_Bitmap_
{
  unsigned int    rows;          ///< The number of bitmap rows. [lines/pixel]
  unsigned int    width;         ///< The number of pixels in bitmap row. [pixel]
  int             pitch;	     ///< The pitch’s value is the number of bytes taken by one bitmap row, including padding. [pixel]
  unsigned char*  buffer;        ///< A typeless pointer to the bitmap buffer. This value should be aligned on 8-bit boundaries.
  unsigned short  num_grays;     ///< This field is only used with FT_PIXEL_MODE_GRAY; it gives the number of gray levels used in the bitmap.
  unsigned char   pixel_mode;    ///< The pixel mode, i.e., how pixel bits are stored. See FT_Pixel_Mode for possible values.
  unsigned char   palette_mode;  ///< This field is intended for paletted pixel modes; it indicates how the palette is stored. Not used currently.
  void*           palette;       ///< A typeless pointer to the bitmap palette; this field is intended for paletted pixel modes. Not used currently.
};

#ifdef __cplusplus
}
#endif
