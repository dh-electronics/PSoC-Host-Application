#include <c-api/bitmap.h>
#include <string.h>


Bitmap::Bitmap(int w, int h, const unsigned char *bytes)
    : width(w)
    , height(h)
    , pitch((width >> 3) + (w & 0x07 ? 1 : 0)) // padding to meet byte boundary
    , size(pitch * height)
{
    data = new unsigned char[size];
    if(bytes)
        memcpy(data, bytes, size);
}


Bitmap::Bitmap(const Bitmap &other)
    : width(other.width)
    , height(other.height)
    , pitch(other.pitch)
    , size(other.size)
{
    data = new unsigned char[size];
    memcpy(data, other.data, size);
}


Bitmap::~Bitmap()
{
    delete data;
}
