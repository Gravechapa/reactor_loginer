#pragma once
#include <curl/curl.h>
#include <stdexcept>

class CurlWrapper
{
public:
    CurlWrapper();
    ~CurlWrapper();

    CurlWrapper duphandle() const;
    void perform() const;

    template <typename... Ts>
    void setOpt(CURLoption option, Ts... args) const
    {
        auto status = curl_easy_setopt(_curl, option, args...);
        if (status != CURLE_OK)
        {
            throw std::runtime_error("Error occurred while curl setopt, code: "
                                     + std::string(curl_easy_strerror(status)));
        }
    }

    template <typename... Ts>
    void getInfo(CURLINFO info, Ts... args) const
    {
        auto status = curl_easy_getinfo(_curl, info, args...);
        if (status != CURLE_OK)
        {
            throw std::runtime_error("Error occurred while curl getinfo, code: "
                                     + std::string(curl_easy_strerror(status)));
        }
    }

private:
    CurlWrapper(CURL * const curl);

    CURL * const _curl;
};
