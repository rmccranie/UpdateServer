#include "utils.h"
#include <sys/time.h>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <settings.h>
#include <sstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <algorithm>    // copy
#include <iterator>     // ostream_operator

#include <boost/tokenizer.hpp>



using namespace std ;

const std::string Utils::currentDateTime() {

    char            fmt[64], buf[80];
    struct timeval  tv;

    struct tm*  tstruct;

    gettimeofday(&tv, NULL);
    if((tstruct = localtime(&tv.tv_sec)) != NULL)
    {
        strftime(fmt, sizeof fmt, "%Y-%m-%d %H:%M:%S.%%06u", tstruct);
        snprintf(buf, sizeof buf, fmt, tv.tv_usec);
    }

    return buf;
}

bool Utils::loadIniFile ()
{
    boost::property_tree::ptree pt;
    try 
    {
       Settings::cleanParams() ;
       boost::property_tree::ini_parser::read_ini(Settings::getIniFilename().c_str(), pt);

       //-- Load server parameters.
       Settings::setUpdatePolicy (pt.get<std::string>("ServerOps.us_update_policy"));
       Settings::setLogPath (pt.get<std::string>("ServerOps.us_log_directory"));

       //-- Load client parameters.
       string allVersion         = pt.get<std::string>("ClientAllGroup.us_client_version")  ;
       string allUpdateUrl       = pt.get<std::string>("ClientAllGroup.us_client_update_url") ;
       int    allUpdateInterval  = atoi((pt.get<std::string>("ClientAllGroup.us_client_update_interval")).c_str()) ;
       Settings::setClientParams ("all", allVersion, allUpdateUrl, allUpdateInterval) ;

       cout << "version (all): " << allVersion << endl ;

       string evenVersion        = pt.get<std::string>("ClientEvenGroup.us_client_version")  ;
       string evenUpdateUrl      = pt.get<std::string>("ClientEvenGroup.us_client_update_url") ;
       int    evenUpdateInterval = atoi((pt.get<std::string>("ClientEvenGroup.us_client_update_interval")).c_str()) ;
       Settings::setClientParams ("even", evenVersion, evenUpdateUrl, evenUpdateInterval) ;

       string oddVersion         = pt.get<std::string>("ClientOddGroup.us_client_version")  ;
       string oddUpdateUrl       = pt.get<std::string>("ClientOddGroup.us_client_update_url") ;
       int    oddUpdateInterval  = atoi((pt.get<std::string>("ClientOddGroup.us_client_update_interval")).c_str()) ;
       Settings::setClientParams ("odd", oddVersion, oddUpdateUrl, oddUpdateInterval) ;

    }
    catch (exception e)
    {
        cout << "Exception: " << e.what() << endl;
    }

    return true;
}
