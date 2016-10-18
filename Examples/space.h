#ifndef SPACE_H
#define SPACE_H


#include "bitmap.h"
#include "buttons.h"


class Space
{
public:
    static void run();

private:
    static buttonsCallback callback;
    static bool running_;
};

#endif // SPACE_H
