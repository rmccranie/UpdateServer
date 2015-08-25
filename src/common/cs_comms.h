#ifndef CS_COMMS_H
#define CS_COMMS_H

#include <vector>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>
#include <inttypes.h>

using namespace std;

#define MSGSZ     128

//-- Define message structure.
typedef enum CommandType   { start, stop, quit }    CommandType;
typedef enum OperationType { simple, medium, hard } OperationType;

typedef struct message_buf
{
    long          m_type;
    OperationType op_type;
    CommandType   cmd_type;
    int           num1;
    int           num2;
    char          mtext[MSGSZ];
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
