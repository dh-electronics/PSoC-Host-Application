#include <leds.h>
#include <helpers.h>
#include <unistd.h>
#include <iostream>


using namespace std;


void testLeds()
{
    cout << "\nTesting leds ..." << endl;

    RESULT res;
    for(int i = 0; i < 1000; ++i)
    {
        for(int l = 0; l <= LED_ERR; ++l)
        {
            const bool newState =  i & (1 << l);
            const LED led = LED(l);
            const bool oldState = getLedState(led, &res);
            if(RESULT_OK != res)
            {
                cerr << "getLedState: " << getResultCodeString(res) << endl;
                break;
            }

            if(newState == oldState)
                continue;

            res = writeLed(led, newState);
            if(RESULT_OK != res)
            {
                cerr << "writeLed: " << getResultCodeString(res) << endl;
                break;
            }
        }
        if(RESULT_OK != res)
            break;

        usleep(10000);
    }

    for(int l = 0; l <= LED_ERR; ++l)
        writeLed(LED(l), false);

    cout << "... done." << endl;
}
