
#include <c-api/helpers.h>
#include <c-api/stats.h>
#include <c-api/init.h>
#include <stdlib.h>
#include <iostream>


using namespace std;


void testDisplay();
void testLeds();
void testButtons();
void testSplash();
int infiniteDisplayButtonsTest();


int main(int, char **)
{
    RESULT res = openApi();
    if(RESULT_OK != res)
    {
        cout << "\nError opening Api: " << getResultCodeString(res) << endl;
        exit(EXIT_FAILURE);
    }


    cout << "HW Rev: " << getHwRevision() << hex << ", API: " << getApiVersion() << ", FW: " << getPicVersion() << dec << endl;
    resetStats();

    testSplash();
    testButtons();
    testDisplay();
    testLeds();


    cout << "\nCrcErrorCounter:\t" << getCrcErrorCounter() <<
            "\nResponseErrorCounter:\t" << getResponseErrorCounter() <<
            "\nBusyCounter:\t\t" << getBusyCounter() <<
            "\nTimeoutsCounter:\t" << getTimeoutsCounter() <<
            "\nResetsCounter\t" << getResetsCounter() << endl;

    infiniteDisplayButtonsTest();

    res = closeApi();
    if(RESULT_OK != res)
    {
        cout << "Error closing Api: " << getResultCodeString(res) << endl;
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
