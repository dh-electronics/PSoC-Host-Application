#include <c-api/display.h>
#include <c-api/helpers.h>
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

    cout << "\nTesting contrast ... skipped (blocked in latest Firmware)" << endl;
    displayFill(true);
    displayFlush();
/*
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
*/

    // should pass
    res = displaySetDimTimeout(300);
    if(RESULT_OK != res)
    {
        cerr << "displaySetDimTimeout: " << getResultCodeString(res) << endl;
    }
    res = displaySetOffTimeout(600);
    if(RESULT_OK != res)
    {
        cerr << "displaySetOffTimeout: " << getResultCodeString(res) << endl;
    }

/*
    // should fail
    res = displaySetDimTimeout(301);
    if(RESULT_OK != res)
    {
        cerr << "displaySetDimTimeout: " << getResultCodeString(res) << endl;
    }
    res = displaySetOffTimeout(601);
    if(RESULT_OK != res)
    {
        cerr << "displaySetOffTimeout: " << getResultCodeString(res) << endl;
    }
*/

    res = displaySetDimTimeout(5);
    if(RESULT_OK != res)
    {
        cerr << "displaySetDimTimeout: " << getResultCodeString(res) << endl;
    }
    res = displaySetOffTimeout(10);
    if(RESULT_OK != res)
    {
        cerr << "displaySetOffTimeout: " << getResultCodeString(res) << endl;
    }

    sleep(20);

    res = displayEnable(false);
    if(RESULT_OK != res)
        cerr << "displayEnable: " << getResultCodeString(res) << endl;

    cout << "... done." << endl;
}
