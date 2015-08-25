#ifndef UPDATE_SERVER_H
#define UPDATE_SERVER_H

#include <iostream>
#include <csignal>
#include <fstream>
#include "settings.h"
#include "cs_comms.h"

using namespace std;

const int timeout = 60 ;
const int numberOfChildren = 10;

class UpdateServer
{
public:
    UpdateServer() ;
    ~UpdateServer() ;
    int Run();
    static bool SetRunning (bool set) { running = set ; }
private:
    int children ;
    pid_t serverPid ;
    ofstream myFile ;
//    ClientServerComms * cs_comms ;
    static bool running ;
     //-- Private functions
    bool UpdateAvailable() ;
    
};

#endif
