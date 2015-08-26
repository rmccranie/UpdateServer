#include "client.h"
#include "utils.h"
#include <sys/prctl.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdio>
#include <cstdlib>

#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <netinet/in.h>
#include <resolv.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>




using namespace std;

UpdateClient::UpdateClient ( int sn ) :
clientSerialNum (sn)
{

}

bool UpdateClient::DoUpdate (int ver)
{

    currentVersion = ver ;

    return true ;
}

void UpdateClient::HandleReceivedMessage ( message_buf * buf )
{
    //-- always set the update interval regardless of messate type.  Server knows best.
    updateInterval = buf->updateInterval ;
    cout << "Update interval set to: " << updateInterval << endl ;

    //- Hanndle messages by type.
    switch ( buf->msg_type )
    {
    case sc_doUpdate:
        cout << "Commanded to update.  UPDATING!" << endl ;
        DoUpdate( buf->clientVersion );
        break ;
    case sc_noUpdateAvailable:
        cout << "No update available for this period." << endl ;
        break ;
    default:
        cout << "Invalid message type!" << endl ;
   } 
}

int UpdateClient::Run ()
{
    int host_port= serverPort;
    char host_name[]="127.0.0.1";

    struct sockaddr_in my_addr;

    message_buf buffer ;
    int bytecount;
    int buffer_len=0;

    int hsock;
    int * p_int;
    int err;

    hsock = socket(AF_INET, SOCK_STREAM, 0);
    if(hsock == -1){
        printf("Error initializing socket %d\n",errno);
        return 0;
    }
    
    p_int = (int*)malloc(sizeof(int));
    *p_int = 1;
        
    if( (setsockopt(hsock, SOL_SOCKET, SO_REUSEADDR, (char*)p_int, sizeof(int)) == -1 )||
        (setsockopt(hsock, SOL_SOCKET, SO_KEEPALIVE, (char*)p_int, sizeof(int)) == -1 ) ){
        printf("Error setting options %d\n",errno);
        free(p_int);
        return 0 ;
    }
    free(p_int);

    my_addr.sin_family = AF_INET ;
    my_addr.sin_port = htons(host_port);
    
    memset(&(my_addr.sin_zero), 0, 8);
    my_addr.sin_addr.s_addr = inet_addr(host_name);

    if( connect( hsock, (struct sockaddr*)&my_addr, sizeof(my_addr)) == -1 ){
        if((err = errno) != EINPROGRESS){
            fprintf(stderr, "Error connecting socket %d\n", errno);
            return 0;
        }
    }

    //Now lets do the client related stuff

    buffer_len = sizeof (message_buf);

    memset(&buffer, '\0', buffer_len);

    buffer.msg_type = cs_updateAvailable ;
    buffer.clientVersion = invalidVersion ; 
    buffer.clientSerial = clientSerialNum ;

    if( (bytecount=send(hsock, &buffer, buffer_len,0))== -1){
        fprintf(stderr, "Error sending data %d\n", errno);
        return 0 ;
    }

    if((bytecount = recv(hsock, &buffer, buffer_len, 0))== -1){
        fprintf(stderr, "Error receiving data %d\n", errno);
        return 0 ;
    }

    cout << "Received - msg_type: " << ClientServerComms::GetMessageTypeString(buffer.msg_type) << ", version; " << buffer.clientVersion << endl ;

    HandleReceivedMessage (&buffer) ;
    close(hsock);
}
