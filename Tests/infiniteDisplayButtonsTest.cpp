#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <buttons.h>
#include <display.h>


#define NUMBER_OF_BITMAPS 2

#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64
#define DISPLAY_PITCH 16
#define DISPLAY_MODE 1 // FT_PIXEL_MODE_MONO


typedef struct  FT_Bitmap_
{
    unsigned int    rows;
    unsigned int    width;
    int             pitch;
    unsigned char*  buffer;
    unsigned short  num_grays;
    unsigned char   pixel_mode;
    unsigned char   palette_mode;
    void*           palette;

} FT_Bitmap;


void globalButtonsCallback(BUTTON button, bool pressed)
{
    printf("Button %d %s\n", button, pressed ? "pressed" : "released");
    fflush(stdout);
}


int infiniteDisplayButtonsTest()
{
    printf("\nInfinite display test 3 (Arigo)\n");
    setButtonsCallback(&globalButtonsCallback);
    displayEnable(true);

    FT_Bitmap_ bitmaps[NUMBER_OF_BITMAPS];

    for(int index = 0; index < NUMBER_OF_BITMAPS; ++index) {
        bitmaps[index].rows = DISPLAY_HEIGHT;
        bitmaps[index].width = DISPLAY_WIDTH;
        bitmaps[index].pitch = DISPLAY_PITCH;
        bitmaps[index].pixel_mode = DISPLAY_MODE;
        bitmaps[index].buffer = (unsigned char*)malloc(DISPLAY_HEIGHT * DISPLAY_PITCH);
        for(int bufferIndex = 0; DISPLAY_HEIGHT * DISPLAY_PITCH > bufferIndex; ++bufferIndex) {
            bitmaps[index].buffer[bufferIndex] = bufferIndex + index;
        }
    }

    int bitmapIndex(0);

    while(true) {
        handleButtons();

        displayFill(0);
        displayBitmap(0, 0, bitmaps[bitmapIndex]);
        displayFlush();
        ++bitmapIndex;

        if(NUMBER_OF_BITMAPS == bitmapIndex)
            bitmapIndex = 0;
    }
}
