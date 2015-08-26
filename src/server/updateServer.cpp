#include <iostream>
#include "updateServer.h"
#include <sys/prctl.h>
#include <utils.h>
#include <inttypes.h>
#include <boost/filesystem.hpp>
#include <fcntl.h>
#include <cstring>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <netinet/in.h>
#include <resolv.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <sstream>
#include "cs_comms.h"
#include <exception>
using namespace std;
using namespace boost::filesystem;

bool UpdateServer::running = true ;

UpdateServer::UpdateServer() :
listenPort (serverPort)
{
    stringstream filename ;
    filename << Settings::getLogPath() << "/Parent_log.txt" ;
    
    myFile.open (filename.str().c_str(), std::ofstream::out | std::ofstream::app);    
    char mynameis[] = "updateServer" ;
    prctl(PR_SET_NAME, (unsigned long) mynameis, 0, 0, 0);

    memset(&act, 0, sizeof(act));

    //-- Register signal handler for SIGHUP
    act.sa_sigaction = sighandler;
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGHUP, &act, NULL);
} 


//-- REceived SIGHUP, so reload INI file.
//-- NOTE: there is some danger here dependinging on whate's happening
//-- during interrupt time.  It would most likely happen while the 
//-- server is sleeping, but it's possible there could be some problems
//-- here due to that.  For now, it works for this demo purpose.
void UpdateServer::sighandler(int signum, siginfo_t *info, void *ptr)
{
    Utils::loadIniFile() ;
}


UpdateServer::~UpdateServer()
{
    //-- make sure the map gets cleaned up.
    Settings::cleanParams();
}

/*
 * Implementes the main application loop. Loops on accept
 * waiting for client requests.
 */
int UpdateServer::Run()
{
    struct sockaddr_in my_addr;

    int hsock;
    int * p_int ;
    int err;

    socklen_t addr_size = 0;
    int* csock;
    sockaddr_in sadr;
    pthread_t thread_id=0;

    //-- Load
    Utils::loadIniFile() ;

    //-- Set up socket.
    hsock = socket(AF_INET, SOCK_STREAM, 0);
    if(hsock == -1)
    {
        cout << "Error initializing socket " <<  errno << endl;
        return 0 ;
    }
    
    p_int = (int*)malloc(sizeof(int));
    *p_int = 1;
        
    if( (setsockopt(hsock, SOL_SOCKET, SO_REUSEADDR, (char*)p_int, sizeof(int)) == -1 )||
        (setsockopt(hsock, SOL_SOCKET, SO_KEEPALIVE, (char*)p_int, sizeof(int)) == -1 ) )
    {
        cout << "Error setting options " <<  errno << endl;
        free(p_int);
        return 0 ;
    }
    free(p_int);

    my_addr.sin_family = AF_INET ;
    my_addr.sin_port = htons(listenPort);
    
    memset(&(my_addr.sin_zero), 0, 8);
    my_addr.sin_addr.s_addr = INADDR_ANY ;
    
    if( bind( hsock, (sockaddr*)&my_addr, sizeof(my_addr)) == -1 )
    {
        cout << "Error binding to socket, make sure nothing else is listening on this port " << errno << endl ;
        return 0 ;
    }

    if(listen( hsock, 10) == -1 ){
        cout << "Error listening " << errno << endl ;
        return 0 ;
    }

    addr_size = sizeof(sockaddr_in);
   
    //-- loop forever on accept. Spawn a thread to handle the client when a message is available. 
    while ( true )
    {
        cout << "waiting for a connection" << endl ;
        csock = (int*)malloc(sizeof(int));
        if((*csock = accept( hsock, (sockaddr*)&sadr, &addr_size))!= -1)
        {
            thread_params *tp = (thread_params*)malloc(sizeof(thread_params));
            tp->csock = csock ;
            tp->this1 = this ;
            cout << "---------------------\nReceived connection from " << inet_ntoa(sadr.sin_addr) << endl;
            pthread_create(&thread_id, 0, &ClientHandler, (void*)tp );
            pthread_detach(thread_id);
        }
        else
        {
            cout << "Error accepting " <<  errno << endl ;
        }
    }
    
    return 0 ;
}

/*
 * Respond to individual client messages.
 */
void UpdateServer::HandleMessage ( ClientParams * cp, message_buf * buf )
{
    
    buf->updateInterval = cp->GetUpdateInterval () ; 

    //-- At the moment, we only get one message type from clients.
    switch (buf->msg_type)
    {
    //-- all we let the ClientParms determine what to do about update
    //-- policy.  It's just selected appropriately based on that.
    case cs_updateAvailable:
       if ( buf->clientVersion != atoi (cp->GetVersion().c_str()))
       {
           buf->clientVersion = atoi (cp->GetVersion().c_str()) ;
           buf->msg_type = sc_doUpdate ;  
           sprintf (buf->url, cp->GetUpdateUrl().c_str(), strlen(cp->GetUpdateUrl().c_str() )) ;
       }
       else
       {
          buf->msg_type = sc_noUpdateAvailable ;
       }
       break ;

    default: 
       cout << "Invalid message type receieved from client" << endl ;
    } 
}


/*
 * The handler for each thread to use for responding
 * to client requests.
 */
void* UpdateServer::ClientHandler(void* lp)
{
    int *csock = ((thread_params *)lp)->csock;
    UpdateServer *this1 = ((thread_params *)lp)->this1 ; 

    message_buf buffer ;
    int buffer_len = sizeof (message_buf);
    int bytecount;

    memset(&buffer, 0, buffer_len);
    if((bytecount = recv(*csock, &buffer, buffer_len, 0))== -1){
        cout << "Error receiving data " << errno << endl ;
        free (csock) ;
        return 0;
    }

    cout << "Received - msg_type: " << ClientServerComms::GetMessageTypeString(buffer.msg_type) << ", clnt version: " << buffer.clientVersion 
         << " from client: " << buffer.clientSerial << endl ;

    //== Get ClientParams based on update policy in force.
    UpdatePolicy policy = Settings::getUpdatePolicy() ;
    std::string s = (policy == all ? "all" : (policy == odd_even && buffer.clientSerial % 2) ? "odd" : "even") ;
    ClientParams *cp = Settings::getClientParams ( s );

    if ( cp == NULL )
        throw std::runtime_error("Error: call to getClientParams resulted in NULL return");

    //-- call an object method on the caller.
    this1->HandleMessage ( cp, &buffer ) ;

    if((bytecount = send(*csock, &buffer, buffer_len, 0))== -1)
    {
        cout << "Error sending data " <<  errno << endl ;
        free (csock) ;
        return 0 ;
    }
     
    cout << "Sent bytes " <<  bytecount << endl;
}
