#ifndef CLIENT_PARAMS_H
#define CLIENT_PARAMS_H
#include <string>

class ClientParams 
{
public:
    ClientParams ( std::string version, std::string updateUrl, int updateInterval ) ;
    std::string &GetVersion() { return version ; }
    std::string &GetUpdateUrl() { return updateUrl ; }
    int          GetUpdateInterval() { return updateInterval ; }


private:
    std::string version ;
    std::string updateUrl ;
    int updateInterval ; 
} ;



#endif

