#ifndef UTILS_H
#define UTILS_H
#include <string>

class Utils
{
public:

    // Get current date/time, format is YYYY-MM-DD.HH:mm:ss
    static const std::string currentDateTime() ;
    static bool loadIniFile () ;
} ;



#endif
