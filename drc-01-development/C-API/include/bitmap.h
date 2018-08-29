#pragma once


#include <stddef.h>


struct Bitmap
{
    Bitmap(int w, int h, const unsigned char *bytes = NULL);
    Bitmap(const Bitmap &other);
    ~Bitmap();

    const int width;
    const int height;
    const int pitch;
    const int size;
    unsigned char *data;
};


struct  FT_Bitmap_
{
  unsigned int    rows;
  unsigned int    width;
  int             pitch;
  unsigned char*  buffer;
  unsigned short  num_grays;
  unsigned char   pixel_mode;
  unsigned char   palette_mode;
  void*           palette;

};
