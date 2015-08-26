#ifndef UPDATE_SERVER_H
#define UPDATE_SERVER_H

#include <iostream>
#include <csignal>
#include <fstream>
#include "settings.h"
#include "cs_comms.h"

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
    std::ofstream myFile ;
    static bool running ;
    struct sigaction act;

     //-- Private functions
    static void *ClientHandler (void *) ;
    void HandleMessage ( ClientParams *, message_buf * ) ; 
    static void sighandler (int, siginfo_t *, void * ) ;
    
};

#endif
