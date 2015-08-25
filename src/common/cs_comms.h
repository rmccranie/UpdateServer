#ifndef CS_COMMS_H
#define CS_COMMS_H

#include <vector>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>
#include <inttypes.h>
#include <string>

using namespace std;

#define URL_SIZE     128

//-- Define message structure.
typedef enum MessageType   { cs_updateAvailable, sc_doUpdate, sc_reloadIni }    MessageType;
typedef enum UpdatePolicy  { odd_even, all } UpdatePolicy ;

typedef struct message_buf
{
    long          m_type;
    MessageType   msg_type;
    int           clientVersion ;
    char          url[URL_SIZE];
} message_buf;


const key_t queue_key = 1234 ;
const int operation_delay_ns [3] = { 10000000, 100000000, 500000000 } ;
class ClientServerComms
{
public:
    ClientServerComms () ;
    void SendMessage ( int, message_buf  * ) ;
    //-- This is the broadcast method to message all children.
private:
    //-- Encapsulate data needed to communicate with a child.
    //-- an unsigned char will do to hold this for now.
    int messagingPeriod; 
} ;

#endif
