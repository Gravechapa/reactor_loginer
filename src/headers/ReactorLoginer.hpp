#pragma once
#include "Config.hpp"
#include "CurlWrapper.hpp"

class ReactorLoginer
{
public:
    ReactorLoginer(Config &&config);

    [[noreturn]] void start();

private:
    std::string _parseCsrfToken(std::string &html);

    Config _config;
    CurlWrapper _curlConfig;
};
