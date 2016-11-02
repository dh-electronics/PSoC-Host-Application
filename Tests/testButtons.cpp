#include <buttons.h>
#include <helpers.h>
#include <unistd.h>
#include <iostream>


using namespace std;


static char bits;


static void myButtonsCallback(enum BUTTON button, bool pressed)
{
    static const char *buttNames[] = { "ESC", "DOWN", "MID", "OK", "UP" };
    bits |= 1 << button;
    cout << buttNames[button] << (pressed ? " pressed" : " released") << endl;
}


void testButtons()
{
    cout << "\nTesting buttons - touch them ..." << endl;
    bits = 0;

    handleButtons();

    RESULT res = setButtonsCallback(myButtonsCallback);
    if(RESULT_OK != res)
    {
        cerr << "setButtonsCallback: " << getResultCodeString(res) << endl;
        return;
    }
    else while(bits != 0x1f)
    {
        res = handleButtons();
        if(RESULT_OK != res)
        {
            cerr << "handleButtons: " << getResultCodeString(res) << endl;
            break;
        }
        usleep(25000);
    }

    cout << "... done." << endl;
}
