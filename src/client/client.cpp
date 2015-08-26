#include "client.h"
#include "utils.h"
#include "settings.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
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

/* 
 * Do what the name implies. Uses a TFTODownloader (curl) to 
 * do its thing.
 */
bool UpdateClient::DoUpdate (int ver, const char *url)
{
    TFTPDownloader dl ;
    stringstream filename ;
    filename << "./runtime_client/client_"  << clientSerialNum << "/" << Settings::getFirmwarePath() << "/myfirmware.zip" ;
   
    currentVersion = ver ;

    firmwareFile.open (filename.str().c_str(), ios::out | ios::binary);
 
    dl.Download (url, &firmwareFile ) ; 

    firmwareFile.close() ;
    return true ;
}

/*
 * Act appropriately based on message type (update the message_buf.
 */
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

/*
 * Implement the main client loop.  Do complete socket
 * creation and cleanup each time.
 */
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
    } 
    catch (exception e ) 
    {

        return -1 ;
    }
   
   //-- this is the loop.  After send/receiver, the client
   //-- sleeps a time determined by data supplied by
   //-- the server.
   while ( true ) 
   {
       hsock = socket(AF_INET, SOCK_STREAM, 0);
       if( hsock == -1 )
       {
           cout << "Error initializing socket " << errno << endl ;
           return 0;
       }
    
       p_int = (int*)malloc(sizeof(int));
       *p_int = 1;
        
       if( (setsockopt(hsock, SOL_SOCKET, SO_REUSEADDR, (char*)p_int, sizeof(int)) == -1 )||
           (setsockopt(hsock, SOL_SOCKET, SO_KEEPALIVE, (char*)p_int, sizeof(int)) == -1 ) )
       {
           cout << "Error setting options " << errno << endl ;
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
               cout << "Error connecting socket " <<  errno << endl ;;
               return 0;
           }
       }

       buffer_len = sizeof (message_buf);

       memset(&buffer, '\0', buffer_len);

       buffer.msg_type = cs_updateAvailable ;
       buffer.clientVersion = currentVersion ; 
       buffer.clientSerial = clientSerialNum ;

       if( (bytecount=send(hsock, &buffer, buffer_len,0))== -1 ) 
       {
           cout << "Error sending data " <<  errno << endl ;
           return 0 ;
       }

       if((bytecount = recv(hsock, &buffer, buffer_len, 0))== -1){
           cout << "Error receiving data " << errno << endl ;
           return 0 ;
       }

       cout << "Received - msg_type: " << ClientServerComms::GetMessageTypeString(buffer.msg_type) << ", version; " << buffer.clientVersion << endl ;

       //-- acto on message received from server.
       HandleReceivedMessage (&buffer) ;

       close(hsock);
       sleep (updateInterval) ;
     }


    return 0 ;
}
