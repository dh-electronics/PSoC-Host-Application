#include <c-api/buttons.h>
#include <c-api/helpers.h>
#include <unistd.h>
#include <iostream>


using namespace std;


static char bits;


static void myButtonsCallback(enum BUTTON button, bool pressed)
{
    static const char *buttNames[] = { "DN_LEFT", "DN_RIGHT", "MID", "UP_LEFT", "UP_RIGHT" };
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
