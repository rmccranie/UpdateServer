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
       boost::property_tree::ini_parser::read_ini(Settings::getIniFilename().c_str(), pt);
   
//       Settings::setSendingRate (atoi((pt.get<std::string>("Section1.pc_sending_rate")).c_str()));
//       Settings::setComplexity (pt.get<std::string>("Section1.pc_complexity"));
//       Settings::setLogPath (pt.get<std::string>("Section1.pc_log_directory"));
    }
    catch (exception e)
    {
        cout << "Exception: " << e.what() << endl;
    }

    return true;
}
