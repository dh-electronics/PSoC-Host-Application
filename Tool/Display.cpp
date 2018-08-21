#include "Display.h"
#include <display.h>
#include <bitmap.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <new>
#include <memory>


using namespace std;


static void displayBmp(const char *filename, int x, int y)
{
    unique_ptr <uint8_t[]> image_data;
    int width, height;
    { // read the bitmap to memory
        FILE *f = fopen(filename, "rb");
        if(!f)
        {
            printf("BMP: file does not exist or could not be opened: %s\n", filename);
            return;
        }

        // read the file header
#pragma pack(1)
        struct
        {
            uint8_t magic [2];
            uint32_t file_size;
            uint16_t reserved[2];
            uint32_t offset;
            uint32_t header_size;
            uint32_t width;
            uint32_t height;
            uint16_t color_planes;
            uint16_t bits_per_pixel;
            uint32_t compression;
            uint32_t image_size;
        } header;
#pragma pack()

        if(1 != fread(&header, sizeof(header), 1, f))
        {
            printf("BMP: could not read the file header.\n");
            return;
        }

        // check the magic
        if(memcmp(&header.magic, "BM", sizeof(header.magic)) != 0)
        {
            printf("BMP: file magic does not match.\n");
            return;
        }

        // must be monochrome
        if(header.bits_per_pixel != 1)
        {
            printf("BMP: must be monochrome bmp.\n");
            return;
        }

        if(header.compression != 0)
        {
            printf("BMP: must be not comressed.\n");
            return;
        }

        width = header.width;
        height = header.height;

        // read data
        const uint32_t pixels = header.width * header.height;
        const uint32_t bytes = (pixels >> 3) + (pixels & 7 ? 1 : 0);
        image_data = unique_ptr <uint8_t[]> (new (nothrow) uint8_t[bytes]);
        if(!image_data)
        {
            printf("BMP: could not reserve memory.\n");
            return;
        }

        fseek(f, header.offset, SEEK_SET);
        if(1 != fread(image_data.get(), bytes, 1, f))
        {
            printf("BMP: could not read image data.\n");
            return;
        }
        fclose (f);
    }

    // create the bitmap and zero the data
    Bitmap bmp(width, height);
    memset(bmp.data, 0, bmp.size);

    // fill the bmp
    for(int y = 0; y < height; ++y)
    {
        const uint32_t src_offset = y * width;
        const uint32_t dst_offset = y * bmp.pitch * 8;
        for(int x = 0; x < width; ++x)
        {
            bool bitval;
            { // getting the bit value from image
                const uint32_t bitoffset = src_offset + x;
                const uint8_t bitmask = 1 << (bitoffset & 7);
                bitval = image_data[bitoffset >> 3] & bitmask;
            }
            const uint32_t bitoffset = dst_offset + x;
            const uint8_t bitmask = 1 << (bitoffset & 7);
            uint8_t &data = bmp.data[bitoffset >> 3];
            if(bitval)
                data |= bitmask;
            else
                data &= ~bitmask;
        }
    }

    // write the bitmap to display
    displayBitmap(x, y, bmp);
}



bool Display::parseArgs(int argc, char **argv, uint16_t &idx)
{
    if(0 != strcmp(argv[idx], "display"))
        return false;

    ++idx;
    while(idx < argc)
    {
        if(0 == strcmp(argv[idx], "enable"))
        {
            ++idx;
            displayEnable(true);
            displaySetContrast(0xff);
        }
        else if(0 == strcmp(argv[idx], "disable"))
        {
            ++idx;
            displayEnable(false);
        }
        else if(0 == strcmp(argv[idx], "fill"))
        {
            ++idx;
            displayFill(getInt(argc, argv, idx, 0));
        }
        else if(0 == strcmp(argv[idx], "fillrect"))
        {
            ++idx;
            displayFillRect(
                        getInt(argc, argv, idx, 60),
                        getInt(argc, argv, idx, 28),
                        getInt(argc, argv, idx, 8),
                        getInt(argc, argv, idx, 8),
                        getInt(argc, argv, idx, 1));
        }
        else if(0 == strcmp(argv[idx], "bmp"))
        {
            ++idx;
            const char *filename = getString(argc, argv, idx);
            if(filename)
                displayBmp( filename,
                            getInt(argc, argv, idx, 0),
                            getInt(argc, argv, idx, 0));
            else
                printf("BMP: filename must be supplied.");
        }
        else if(0 == strcmp(argv[idx], "show"))
        {
            ++idx;
            displayFlush();
        }
        else if(0 == strcmp(argv[idx], "flash"))
        {
            ++idx;
            displayWriteSplash();
        }
        else
        {
            break;
        }
    }
    return true;
}
