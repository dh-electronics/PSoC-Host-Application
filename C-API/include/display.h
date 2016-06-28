#pragma once


#include "resultcodes.h"


struct Bitmap
{
    Bitmap(int w, int h);
    Bitmap(const Bitmap &other);
    ~Bitmap();

    const int width;
    const int height;
    unsigned char *data;
};


struct FT_Bitmap;


enum RESULT displayEnable(int on);


enum RESULT displayFill(bool white = false);


enum RESULT displayFillRect(int x, int y, int w, int h, bool white = true);


enum RESULT displayDrawRect(int x, int y, int w, int h, bool white = true);


enum RESULT displayInvertRect(int x, int y, int w, int h);


enum RESULT displayBitmap(int x, int y, const Bitmap &bmp);
enum RESULT displayBitmap(int x, int y, const FT_Bitmap &bmp);


enum RESULT displayFlush();


enum RESULT displaySwap();
