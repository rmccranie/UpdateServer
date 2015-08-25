#include "cs_comms.h"
#include <cstring>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

ClientServerComms::ClientServerComms ( ) 
{

}

void ClientServerComms::SendMessage ( int mq, message_buf *msg )
{
    int buf_length = sizeof (message_buf) ;

    /*
     * Send a message.
     */
}

