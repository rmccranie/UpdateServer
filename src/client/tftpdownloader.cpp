#include "tftpdownloader.h"
#include <curl/curl.h>
#include <curl/easy.h>
#include <curl/curlbuild.h>
#include <exception>
#include <stdexcept>

TFTPDownloader::TFTPDownloader() :
curl (NULL)
{

    curl = curl_easy_init();

    if (!curl)
        throw std::runtime_error("Error: call to getClientParams resulted in NULL return");
}

TFTPDownloader::~TFTPDownloader()
{

    curl_easy_cleanup(curl);

}


void TFTPDownloader::Download (const char *url, const std::ofstream *outfile)
{
    CURLcode res;
    std::cout << "download url: " << url << std::endl ;
    //-- set curl options.
    curl_easy_setopt(curl, CURLOPT_URL, url ) ;
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Write);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, outfile);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
 
    res = curl_easy_perform(curl);
 
    if(CURLE_OK != res) 
    {
      std::cout << "curl told us " << res << std::endl;
    }
}

size_t TFTPDownloader::Write (void *data, size_t size, size_t nmemb, void *outstream)
{
    ((std::ofstream*) outstream)->write( (const char*)data , size*nmemb) ;
    return size * nmemb;
}
