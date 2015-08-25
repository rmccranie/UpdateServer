#ifndef UPDATE_CLIENT_H
#define UPDATE_CLIENT_H

#include <iostream>
#include "settings.h"
#include <fstream>
#include <inttypes.h>

using namespace std ;

class UpdateClient
{

public: 
    UpdateClient();
    int Run() ;

private:
    pid_t myPid ;
    pid_t parentPid ;
    int childNum;
    int64_t parentPointer ;
    ofstream myFile;
} ;

#endif
