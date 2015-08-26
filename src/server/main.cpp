
#include <iostream>
#include "updateServer.h"
#include "utils.h"

using namespace std;

int main ()
{
    UpdateServer *p = new UpdateServer() ;
    Utils::loadIniFile() ;
    try {
      p->Run();
    }
    catch ( exception e )
    {
        cout << "Exception: " << e.what() << endl ;
    }

    return 0 ;
}

