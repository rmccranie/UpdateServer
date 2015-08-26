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

class UpdateServer ;

typedef struct thread_params
{
    int          * csock ;
    UpdateServer * this1 ;
} thread_params;


class UpdateServer
{
public:
    UpdateServer() ;
    ~UpdateServer() ;
    int Run();
    static bool SetRunning (bool set) { running = set ; }
private:
    int listenPort ;
    pid_t serverPid ;
    ofstream myFile ;
    static bool running ;
     //-- Private functions
    bool UpdateAvailable() ;
    static void *ClientHandler (void *) ;
    void HandleMessage ( ClientParams *, message_buf * ) ; 
    
};

#endif
