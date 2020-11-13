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

    
    displayFill(true);
    displayFlush();

    cout << "\nTesting OLed Saver ... DimTimeout == 5, OffTimeout == 10 sec" << endl;
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

    cout << "\nDisable OLed Screen Saver ... set contrast is allowed!" << endl;
    res = displaySetOLedSaver(0);
    if(RESULT_OK != res)
    {
        cerr << "displaySetOLedSaver: " << getResultCodeString(res) << endl;
    }

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

    cout << "\nEnable OLed Screen Saver ... set contrast is denied again!" << endl;
    res = displaySetOLedSaver(1);
    if(RESULT_OK != res)
    {
        cerr << "displaySetOLedSaver: " << getResultCodeString(res) << endl;
    }
/*
    // this !has! to fail
    res = displaySetContrast(0xff);
    if(RESULT_OK != res)
    {
        cerr << endl << "displaySetContrast: " << getResultCodeString(res) << endl << endl;
    }
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
        cerr << endl << "displaySetDimTimeout: " << getResultCodeString(res) << endl << endl;
    }
    res = displaySetOffTimeout(601);
    if(RESULT_OK != res)
    {
        cerr << endl << "displaySetOffTimeout: " << getResultCodeString(res) << endl << endl;
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
