
#include <iostream>
#include "updateServer.h"
#include "utils.h"

using namespace std;

int main ()
{
    UpdateServer *p = new UpdateServer() ;
    Utils::loadIniFile() ;
    //p->Run();

    return 0 ;
}

