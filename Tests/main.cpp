#include <init.h>
#include <helpers.h>
#include <stats.h>
#include <stdlib.h>
#include <iostream>


using namespace std;


void testDisplay();
void testLeds();
void testButtons();
void testSplash();


int main(int argc, char **argv)
{
    RESULT res = openApi();
    if(RESULT_OK != res)
    {
        cout << "\nError opening Api: " << getResultCodeString(res) << endl;
        exit(EXIT_FAILURE);
    }


    cout << "HW Rev: " << getHwRevision() << hex << ", API: " << getApiVersion() << ", PIC: " << getPicVersion() << dec << endl;
    resetStats();

    testSplash();
    // testDisplay();
    // testButtons();
    // testLeds();

    cout << "\nCrcErrorCounter:\t" << getCrcErrorCounter() <<
            "\nResponseErrorCounter:\t" << getResponseErrorCounter() <<
            "\nBusyCounter:\t\t" << getBusyCounter() <<
            "\nTimeoutsCounter:\t" << getTimeoutsCounter() <<
            "\nResetsCounter\t" << getResetsCounter() << endl;

    res = closeApi();
    if(RESULT_OK != res)
    {
        cout << "Error closing Api: " << getResultCodeString(res) << endl;
        exit(EXIT_FAILURE);
    }
    return EXIT_SUCCESS;
}
