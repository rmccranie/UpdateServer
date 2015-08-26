#ifndef UPDATE_CLIENT_H
#define UPDATE_CLIENT_H

#include <iostream>
#include <fstream>
#include <inttypes.h>
#include "cs_comms.h"

using namespace std ;

class UpdateClient
{

public: 
    UpdateClient();
    int Run() ;

private:
    int currentVersion;
    int updateInterval ;
    ofstream myFile;

    bool DoUpdate (int) ; 
    void HandleReceivedMessage (message_buf *) ;
} ;

#endif
