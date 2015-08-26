#ifndef TFTP_DOWNLOADER_H
#define TFTP_DOWNLOADER_H

#include <string>
#include <iostream>
#include <fstream>



class TFTPDownloader
{
public:
    TFTPDownloader() ;
    ~TFTPDownloader() ;

    void Download (const char *url, const std::ofstream *outfile) ;

private:

    void* curl;
    static size_t Write (void *ptr, size_t size, size_t nmemb, void *outstream);
} ;





#endif
