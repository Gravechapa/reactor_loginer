#include "CurlWrapper.hpp"
#include <iostream>
#include <thread>

CurlWrapper::CurlWrapper(): _curl(curl_easy_init())
{
    if (!_curl)
    {
        throw std::runtime_error("Cant init curl");
    }
}

CurlWrapper::CurlWrapper(CURL * const curl): _curl(curl)
{
}

CurlWrapper::~CurlWrapper()
{
    curl_easy_cleanup(_curl);
}


CurlWrapper CurlWrapper::duphandle() const
{
    auto curl = curl_easy_duphandle(_curl);
    if (!curl)
    {
        throw std::runtime_error("Cant duphandle curl");
    }
    return CurlWrapper(curl);
}

void CurlWrapper::perform() const
{
    short counter = 0;
    CURLcode result;
    while((result = curl_easy_perform(_curl)) != CURLE_OK)
        {
            if (++counter > 10)
                {
                    throw std::runtime_error("Curl error: "
                                             + std::string(curl_easy_strerror(result)));
                }
            std::cout << "Curl issue: " << curl_easy_strerror(result) <<
                         " Retrying: " << counter << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
}
