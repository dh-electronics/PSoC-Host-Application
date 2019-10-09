#include <c-api/display.h>
#include <c-api/helpers.h>
#include <iostream>
#include <stdio.h>
#include <stdint.h>


using namespace std;


void testSplash()
{
    cout << "\nWrite splash? (y/n) ..." << endl;
    if(toupper(getchar()) != 'Y')
        return;

    RESULT res = displayEnable(true);
    if(RESULT_OK != res)
    {
        cerr << "displayEnable: " << getResultCodeString(res) << endl;
        return;
    }

    res = displayFill();
    if(RESULT_OK != res)
        cerr << "displayFill: " << getResultCodeString(res) << endl;

    for(uint8_t x = 0; x < 128; x += 16)
    {
        for(uint8_t y = 0; y < 64; y += 16)
        {
            res = displayDrawRect(x, y, 11, 11);
            if(RESULT_OK != res)
            {
                cerr << "displayDrawRect: " << getResultCodeString(res) << endl;
                break;
            }
        }
        if(RESULT_OK != res)
            break;
    }

    if(RESULT_OK == res)
        res = displayWriteSplash();
    if(RESULT_OK != res)
        cerr << "displayWriteSplash: " << getResultCodeString(res) << endl;

    res = displayEnable(false);
    if(RESULT_OK != res)
        cerr << "displayEnable: " << getResultCodeString(res) << endl;

    cout << "... done." << endl;
}
