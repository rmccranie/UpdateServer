#ifndef UPDATE_CLIENT_H
#define UPDATE_CLIENT_H

#include <iostream>
#include <fstream>
#include <inttypes.h>
#include "cs_comms.h"


class UpdateClient
{

public: 
    UpdateClient(int);
    int Run() ;

private:
    int clientSerialNum ;
    int currentVersion;
    int updateInterval ;
    std::ofstream firmwareFile;

    bool DoUpdate (int, const char*) ; 
    void HandleReceivedMessage (message_buf *) ;
} ;

#endif
