
#include <iostream>
#include "client.h"
#include "utils.h"
#include <stdlib.h>

using namespace std;

int main ( int argc, char** argv )
{
    if ( argc != 2 ) 
    {
        cout << "Incorrect number of command line parameters!" << endl ;
        return -1 ;
    }

    cout << "I am client " << argv[1] << endl;

    UpdateClient *p = new UpdateClient(atoi(argv[1])) ;
    p->Run();

    return 0 ;
}

