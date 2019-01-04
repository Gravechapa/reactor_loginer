#include <iostream>
#include "headers/Config.hpp"
#include "ReactorLoginer.hpp"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <thread>

static pid_t pid = 0;

int main()
{
    signal(SIGINT, [](int) {
            printf("SIGINT got\n");
            if (pid)
            {
                kill(pid, SIGINT);
            }
            exit(0);
        });
    while ((pid = fork()) > 0)
    {
        std::cout << "Daemon started" << std::endl;
        wait(NULL);
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
    switch(pid)
    {
      case -1:
          std::cerr << "Fork fail. Daemon stoped" << std::endl;
          exit(1);
      case 0:
          try
              {
                  ReactorLoginer reactorLoginer(Config ("configs/config.json"));
                  std::cout << "Loginer started" << std::endl;
                  reactorLoginer.start();
              }
          catch (std::exception& e)
              {
                  std::cerr << e.what() << std::endl;
              }
    }

    return 0;
}
