#pragma once


struct Bitmap
{
    Bitmap(int w, int h);
    Bitmap(const Bitmap &other);
    ~Bitmap();

    const int width;
    const int height;
    const int pitch;
    const int size;
    unsigned char *data;
};
