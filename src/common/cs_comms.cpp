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

std::string ClientServerComms::GetMessageTypeString ( MessageType m )
{
    return MessageTypeString[m] ;
}
