#pragma once


#include "resultcodes.h"


struct Bitmap;
struct FT_Bitmap_;


enum RESULT displayEnable(int on);


enum RESULT displaySetContrast(int value);


enum RESULT displayFill(bool white = false);


enum RESULT displayFillRect(int x, int y, int w, int h, bool white = true);


enum RESULT displayDrawRect(int x, int y, int w, int h, bool white = true);


enum RESULT displayInvertRect(int x, int y, int w, int h);


enum RESULT displayBitmap(int x, int y, const Bitmap &bmp);
enum RESULT displayBitmap(int x, int y, const struct FT_Bitmap_ &bmp);


enum RESULT displayFlush();


enum RESULT displaySwap();
enum RESULT displayWriteSplash();
