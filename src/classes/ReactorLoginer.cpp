#include "ReactorLoginer.hpp"
#include <thread>
#include "MyHTMLWrapper.hpp"
#include <iostream>
#include <ctime>

size_t WriteCallback(char *contents, size_t size, size_t nmemb, void *userp)
{
    static_cast<std::string*>(userp)->append(contents, size * nmemb);
    return size * nmemb;
}

ReactorLoginer::ReactorLoginer(Config &&config): _config(std::move(config))
{
    _curlConfig.setOpt(CURLOPT_WRITEFUNCTION, WriteCallback);
    _curlConfig.setOpt(CURLOPT_FOLLOWLOCATION, 0L);
    _curlConfig.setOpt(CURLOPT_COOKIEFILE, "");
    _curlConfig.setOpt(CURLOPT_COOKIEJAR, "");
}

void ReactorLoginer::start()
{
    while (true)
    {
        {
            auto curl = _curlConfig.duphandle();

            curl.setOpt(CURLOPT_URL, "http://joyreactor.cc/login");

            std::string html;
            curl.setOpt(CURLOPT_WRITEDATA, &html);

            curl.perform();

            auto csrfToken = _parseCsrfToken(html);

            html.clear();
            std::string data = "signin%5Busername%5D=" + _config.getLogin()
                    +"&signin%5Bpassword%5D=" + _config.getPassword()
                    +"&signin%5B_csrf_token%5D=" + csrfToken;
            curl.setOpt(CURLOPT_URL, "http://joyreactor.cc/login");
            curl.setOpt(CURLOPT_POST, 1L);
            curl.setOpt(CURLOPT_POSTFIELDS, data.c_str());

            curl.perform();

            long code;
            curl.getInfo(CURLINFO_RESPONSE_CODE, &code);
            if (code != 302)
            {
                throw std::runtime_error("Can't login, code: " + std::to_string(code));
            }

            html.clear();
            curl.setOpt(CURLOPT_URL, "http://joyreactor.cc");
            curl.setOpt(CURLOPT_POST, 0L);

            curl.perform();

            curl.getInfo(CURLINFO_RESPONSE_CODE, &code);
            if (code != 200)
            {
                throw std::runtime_error("Can't open main page, code: " + std::to_string(code));
            }
        }
        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);
        auto localTime = localtime(&in_time_t);
        localTime->tm_hour = 1;
        localTime->tm_min = 0;
        localTime->tm_sec = 0;
        localTime->tm_mday += 1;
        in_time_t = mktime(localTime);
        localTime = localtime(&in_time_t);
        char date[50];
        strftime(date, sizeof(date), "%X %x %Z", localTime);
        std::cout << "Logined, next login: " << date << std::endl;
        std::this_thread::sleep_until(std::chrono::system_clock::from_time_t(in_time_t));
    }
}

std::string ReactorLoginer::_parseCsrfToken(std::string &html)
{
    MyHTMLWrapper myhtml(html);
    auto collection = myhtml.getNodesByAttributeValue("name", "signin[_csrf_token]");

    return collection.getAttributeValue("value", 0);
}
