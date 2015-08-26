#ifndef CS_COMMS_H
#define CS_COMMS_H

#include <vector>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>
#include <inttypes.h>
#include <string>

#define URL_SIZE     512

const int serverPort = 55123 ;
const int invalidVersion = -1;

//-- Define message structure.
typedef enum MessageType   { cs_updateAvailable, sc_noUpdateAvailable, sc_doUpdate, messageTypeMax  }    MessageType;
typedef enum UpdatePolicy  { odd_even, all } UpdatePolicy ;

typedef struct message_buf
{
    MessageType   msg_type;
    int           clientSerial ;
    int           clientVersion ;
    int           updateInterval ;
    char          url[URL_SIZE];
} message_buf;


class ClientServerComms
{
public:
    ClientServerComms () ;
    static std::string GetMessageTypeString (MessageType m) ;
    void SendMessage ( int, message_buf  * ) ;
    int GetUpdateInterval () { return updateInterval ; }
    //-- This is the broadcast method to message all children.
private:
    //-- Encapsulate data needed to communicate with a child.
    //-- an unsigned char will do to hold this for now.
    int updateInterval ; 
} ;

#endif
