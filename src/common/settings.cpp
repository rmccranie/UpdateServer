#include "settings.h"

using namespace std ;

string Settings::logPath = "./log";
string Settings::firmwarePath = "./firmware";
string Settings::iniFilename = "./updateServer.ini";
UpdatePolicy Settings::updatePolicy = all ;
std::map<std::string, ClientParams *> Settings::clientParamsMap ;

void Settings::setClientParams ( string key, string ver, string url, int interval )
{

    ClientParams *cp = new ClientParams (ver, url, interval ) ;
    clientParamsMap.insert (std::pair<std::string, ClientParams*>(key, cp)) ;

}

void Settings::setUpdatePolicy (string p )
{
    //-- make the default "all"
    updatePolicy = (p.compare("odd_even") ? odd_even : all ) ;

}

void Settings::cleanParams () 
{
    map<std::string, ClientParams *>::iterator it;

    for (it = clientParamsMap.begin(); it != clientParamsMap.end(); it++)
    {
        delete it->second ;
    }
    clientParamsMap.clear() ;
}

ClientParams *Settings::getClientParams (std::string key)
{
    map<std::string, ClientParams *>::iterator it;

    it = clientParamsMap.find (key) ;
    if ( it == clientParamsMap.end() )
        return NULL ;
    else 
        return it->second ;
}    
