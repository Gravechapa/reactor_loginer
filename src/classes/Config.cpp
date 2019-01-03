#include "Config.hpp"
#include <fstream>

Config::Config(std::string configFile)
{
    std::ifstream config(configFile);
    if (!config.is_open())
        {
            throw std::runtime_error("Can't open config file: " + configFile);
        }
    nlohmann::json json;
    config >> json;

    auto it = json.find("login");
    if (it == json.end())
         {
             throw std::runtime_error("Config file don't have \"login\" field");
         }
    if (!it.value().is_string())
         {
             throw std::runtime_error("Bad config: field \"login\" is not a string");
         }
    _login = it.value().get<std::string>();

    it = json.find("password");
    if (it == json.end())
         {
             throw std::runtime_error("Config file don't have \"password\" field");
         }
    if (!it.value().is_string())
         {
             throw std::runtime_error("Bad config: field \"password\" is not a string");
         }
    _password = it.value().get<std::string>();
}

std::string Config::getLogin() const
{
    return _login;
}

std::string Config::getPassword() const
{
    return _password;
}
