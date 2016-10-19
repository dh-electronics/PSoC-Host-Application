#include <display.h>
#include <helpers.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <iostream>


using namespace std;


void testDisplay()
{
    cout << "\nTesting display ..." << endl;

    srand(time(NULL));

    RESULT res = displayEnable(true);
    if(RESULT_OK != res)
    {
        cerr << "displayEnable: " << getResultCodeString(res) << endl;
        return;
    }

    res = displayFill();
    if(RESULT_OK != res)
        cerr << "displayFill: " << getResultCodeString(res) << endl;

    for(int i = 0; i < 256; ++i)
    {
        res = displayInvertRect(rand() % 178 - 50, rand() % 114 - 50, rand() % 50, rand() % 50);
        if(RESULT_OK != res)
        {
            cerr << "displayInvertRect: " << getResultCodeString(res) << endl;
            break;
        }

        res = displayFlush();
        if(RESULT_OK != res)
        {
            cerr << "displayFlush: " << getResultCodeString(res) << endl;
            break;
        }
    }

    cout << "\nTesting contrast ..." << endl;
    displayFill(true);
    displayFlush();

    for(int i = 0; i < 2048; ++i)
    {
        res = displaySetContrast(i & 0xff);
        if(RESULT_OK != res)
        {
            cerr << "displaySetContrast: " << getResultCodeString(res) << endl;
            break;
        }
        usleep(10000);
    }

    displayFill();
    displayFlush();

    res = displayEnable(false);
    if(RESULT_OK != res)
        cerr << "displayEnable: " << getResultCodeString(res) << endl;

    cout << "... done." << endl;
}
