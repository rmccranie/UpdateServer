#include "client.h"
#include "utils.h"
#include "settings.h"
#include <sys/prctl.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

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

#include <boost/filesystem.hpp>
#include "tftpdownloader.h"

using namespace std;

UpdateClient::UpdateClient ( int sn ) :
clientSerialNum (sn)
{

}

bool UpdateClient::DoUpdate (int ver, const char *url)
{
    TFTPDownloader dl ;
    stringstream filename ;
    filename << "./runtime_client/client_"  << clientSerialNum << "/" << Settings::getFirmwarePath() << "/myfirmware.zip" ;
   
    cout << "url: " << url << endl ; 
    currentVersion = ver ;

    firmwareFile.open (filename.str().c_str(), ios::out | ios::binary);
 
    dl.Download (url, &firmwareFile ) ; 

    firmwareFile.close() ;
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
        DoUpdate( buf->clientVersion, buf->url );
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


    try 
    {
        std::stringstream fullPath ;
        fullPath << "./runtime_client/client_"  << clientSerialNum << "/" << Settings::getFirmwarePath() ;
        boost::filesystem::create_directories (fullPath.str().c_str()) ;
    } catch (exception e ) {

        return -1 ;
    }

   while ( true ) 
   {
       hsock = socket(AF_INET, SOCK_STREAM, 0);
       if( hsock == -1 )
       {
           printf("Error initializing socket %d\n",errno);
           return 0;
       }
    
       p_int = (int*)malloc(sizeof(int));
       *p_int = 1;
        
       if( (setsockopt(hsock, SOL_SOCKET, SO_REUSEADDR, (char*)p_int, sizeof(int)) == -1 )||
           (setsockopt(hsock, SOL_SOCKET, SO_KEEPALIVE, (char*)p_int, sizeof(int)) == -1 ) )
       {
           printf("Error setting options %d\n",errno);
           free(p_int);
           return 0 ;
       }
       
       free(p_int);

       my_addr.sin_family = AF_INET ;
       my_addr.sin_port = htons(host_port);
    
       memset(&(my_addr.sin_zero), 0, 8);
       my_addr.sin_addr.s_addr = inet_addr(host_name);

       if( connect( hsock, (struct sockaddr*)&my_addr, sizeof(my_addr)) == -1 )
       {
           if((err = errno) != EINPROGRESS)
           {
               fprintf(stderr, "Error connecting socket %d\n", errno);
               return 0;
           }
       }

       //Now lets do the client related stuff

       buffer_len = sizeof (message_buf);

       memset(&buffer, '\0', buffer_len);

       buffer.msg_type = cs_updateAvailable ;
       buffer.clientVersion = currentVersion ; 
       buffer.clientSerial = clientSerialNum ;

       cout << "sending" << endl ;
       if( (bytecount=send(hsock, &buffer, buffer_len,0))== -1 ) 
       {
           fprintf(stderr, "Error sending data %d\n", errno);
           return 0 ;
       }

       cout << "receiving" << endl ;
       if((bytecount = recv(hsock, &buffer, buffer_len, 0))== -1){
           fprintf(stderr, "Error receiving data %d\n", errno);
           return 0 ;
       }

       cout << "Received - msg_type: " << ClientServerComms::GetMessageTypeString(buffer.msg_type) << ", version; " << buffer.clientVersion << endl ;

       //-- acto on message received from server.
       HandleReceivedMessage (&buffer) ;

       close(hsock);
       cout << "sleeping" << endl ;
       sleep (updateInterval) ;
       cout << "waking" << endl ;
     }


    return 0 ;
}
