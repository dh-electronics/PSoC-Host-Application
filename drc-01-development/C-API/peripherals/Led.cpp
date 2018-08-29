#include <peripherals/Led.h>
#include <iostream>


using namespace drc01;
using namespace std;


Led::Led()
    : state_(false)
{
}


void Led::restore()
{
    // cout << "Led: restoring value" << endl;
    write(state_);
}
