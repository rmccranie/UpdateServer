#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>

using namespace std ;

/*
 * Encapsulate app config settings.  Eventual idea would be to load these from
 * an ini file.
 */
class Settings
{
public:

    static string &getLogPath()  { return logPath ;  }
    static string &getIniFilename() { return iniFilename ; }
    static void setLogPath(string p)  { logPath = p ;  }
private:
    static string logPath ;
    static string iniFilename ;
};

#endif
