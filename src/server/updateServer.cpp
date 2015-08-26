#include <iostream>
#include "updateServer.h"
#include <sys/prctl.h>
#include <utils.h>
#include <exception> //-- needed because boost lib call can throw exceptions
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
} 

UpdateServer::~UpdateServer()
{

}

bool UpdateAvailable () 
{

    return false ;
}

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


    hsock = socket(AF_INET, SOCK_STREAM, 0);
    if(hsock == -1){
        printf("Error initializing socket %d\n", errno);
        goto FINISH;
    }
    
    p_int = (int*)malloc(sizeof(int));
    *p_int = 1;
        
    if( (setsockopt(hsock, SOL_SOCKET, SO_REUSEADDR, (char*)p_int, sizeof(int)) == -1 )||
        (setsockopt(hsock, SOL_SOCKET, SO_KEEPALIVE, (char*)p_int, sizeof(int)) == -1 ) ){
        printf("Error setting options %d\n", errno);
        free(p_int);
        goto FINISH;
    }
    free(p_int);

    my_addr.sin_family = AF_INET ;
    my_addr.sin_port = htons(listenPort);
    
    memset(&(my_addr.sin_zero), 0, 8);
    my_addr.sin_addr.s_addr = INADDR_ANY ;
    
    if( bind( hsock, (sockaddr*)&my_addr, sizeof(my_addr)) == -1 ){
        fprintf(stderr,"Error binding to socket, make sure nothing else is listening on this port %d\n",errno);
        goto FINISH;
    }
    if(listen( hsock, 10) == -1 ){
        fprintf(stderr, "Error listening %d\n",errno);
        goto FINISH;
    }

    //Now lets do the server stuff

    addr_size = sizeof(sockaddr_in);
    
    while(true){
        printf("waiting for a connection\n");
        csock = (int*)malloc(sizeof(int));
        if((*csock = accept( hsock, (sockaddr*)&sadr, &addr_size))!= -1)
        {
            thread_params *tp = (thread_params*)malloc(sizeof(thread_params));
            tp->csock = csock ;
            tp->this1 = this ;
            printf("---------------------\nReceived connection from %s\n",inet_ntoa(sadr.sin_addr));
            pthread_create(&thread_id, 0, &ClientHandler, (void*)tp );
            pthread_detach(thread_id);
        }
        else{
            fprintf(stderr, "Error accepting %d\n", errno);
        }
    }
    
FINISH:
;
    return 0 ;
}


void UpdateServer::HandleMessage ( ClientParams * cp, message_buf * buf )
{
    
    buf->updateInterval = cp->GetUpdateInterval () ; 

    //-- At the moment, we only get one message type from clients.

    switch (buf->msg_type)
    {
    case cs_updateAvailable:
       buf->clientVersion = atoi (cp->GetVersion().c_str()) ;
       buf->msg_type = sc_doUpdate ;  
       break ;
    default: 
       cout << "Invalid message type receieved from client" << endl ;
    } 
}


void* UpdateServer::ClientHandler(void* lp){
    int *csock = ((thread_params *)lp)->csock;
    UpdateServer *this1 = ((thread_params *)lp)->this1 ; 

    message_buf buffer ;
    int buffer_len = sizeof (message_buf);
    int bytecount;

    memset(&buffer, 0, buffer_len);
    if((bytecount = recv(*csock, &buffer, buffer_len, 0))== -1){
        fprintf(stderr, "Error receiving data %d\n", errno);
        free (csock) ;
        return 0;
    }

    cout << "Received - msg_type: " << ClientServerComms::GetMessageTypeString(buffer.msg_type) << ", clnt version: " << buffer.clientVersion 
         << " from client: " << buffer.clientSerial << endl ;

    //== Get ClientParams based on update policy in force.
    std::string s = "all" ;
    ClientParams *cp = Settings::getClientParams ( s );

    if ( cp == NULL )
        throw std::runtime_error("Error: call to getClientParams resulted in NULL return");

    this1->HandleMessage ( cp, &buffer ) ;

    if((bytecount = send(*csock, &buffer, buffer_len, 0))== -1)
    {
        fprintf(stderr, "Error sending data %d\n", errno);
        free (csock) ;
        return 0 ;
    }
     
    cout << "Sent bytes " <<  bytecount << endl;
 
}
