#include "cs_comms.h"
#include <cstring>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

std::string MessageTypeString [] = { "cs_updateAvailable", "sc_noUpdateAvailable", "sc_doUpdate" } ; 
ClientServerComms::ClientServerComms ( ) 
{
}

void ClientServerComms::SendMessage ( int mq, message_buf *msg )
{

    /*
     * Send a message.
     */
}

std::string ClientServerComms::GetMessageTypeString ( MessageType m )
{
    return MessageTypeString[m] ;
}
