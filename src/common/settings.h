#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>
#include "cs_comms.h"
#include "clientParams.h"
#include <map>
/*
 * Encapsulate app config settings.  Eventual idea would be to load these from
 * an ini file.
 */
class Settings
{
public:

    static std::string &getLogPath()  { return logPath ;  }
    static std::string &getIniFilename() { return iniFilename ; }
    static ClientParams &getClientParams (std::string key) ;
    static void setLogPath(string p)  { logPath = p ;  }
    static void setUpdatePolicy(string p) ;
    static UpdatePolicy &getUpdatePolicy () { return updatePolicy ; }
    static void setClientParams ( std::string, std::string, std::string, int ) ;
    static void cleanParams () ; //-- called to clean up if necessaril.
private:
    static std::string logPath ;
    static std::string iniFilename ;
    static UpdatePolicy updatePolicy ;
    static std::map<std::string,ClientParams *> clientParamsMap;
};

#endif
