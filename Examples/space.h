#ifndef SPACE_H
#define SPACE_H


#include <c-api/bitmap.h>
#include <c-api/buttons.h>


class Space
{
public:
    static void run();

private:
    static buttonsCallback callback;
    static bool running_;
};

#endif // SPACE_H
