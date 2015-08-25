#include "settings.h"

string Settings::logPath = "./log";
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
    
