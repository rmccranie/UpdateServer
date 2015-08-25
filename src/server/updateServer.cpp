#include <iostream>
#include "updateServer.h"
#include <sys/wait.h>
#include <sys/prctl.h>
#include <ctime>
#include <cstring>
#include <sstream>
#include <utils.h>
#include <boost/filesystem.hpp>
#include <sys/stat.h>
#include <unistd.h>
#include <exception> //-- needed because boost lib call can throw exceptions
#include <inttypes.h>

using namespace std;
using namespace boost::filesystem;

bool UpdateServer::running = true ;

UpdateServer::UpdateServer() 
{

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
    return 0 ;
}
