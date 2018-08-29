#include <init.h>
#include <ApiManager.h>


using namespace drc01;
using namespace std;


RESULT openApi()
{
    return ApiManager::openApi();
}


RESULT closeApi()
{
    return ApiManager::closeApi();
}


RESULT idleApi()
{
    IApiManager *man = IApiManager::getIface();
    if(man)
    {
        man->active(false);
        return RESULT_OK;
    }
    else
    {
        return RESULT_API_NOT_OPEN;
    }
}
