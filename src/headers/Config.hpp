#pragma once
#include <nlohmann/json.hpp>
#include <string>

class Config
{
public:
    Config(std::string configFile);

    std::string getLogin() const;
    std::string getPassword() const;

private:
    std::string _login;
    std::string _password;
};
