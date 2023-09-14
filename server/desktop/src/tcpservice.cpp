#include <cctype>
#include <thread>
#include <netdb.h>
#include <cstring>
#include <unistd.h>
#include <iostream>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <QDebug>
#include "tcpservice.h"
#include "setting.h"

void TCPService::run(void)
{
    sockaddr_in servaddr{0};
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(Setting::TCP::PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    // Create socket and check
    int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);

    if (sockfd == -1)
    {
        qDebug() << "Failed to create the socket...\n";
        std::exit(1);
    }

    int tmp = 1;
    if (0 != setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &tmp, sizeof(int)))
    {
        qDebug() << "Failed to set the option .....";
        std::exit(1);
    }

    if (0 != bind(sockfd, (sockaddr *)&servaddr, sizeof(servaddr)))
    {
        qDebug() << "Failed to bind servaddr to the socket...\n";
        std::exit(1);
    }

    if (0 != listen(sockfd, 1))
    {
        qDebug() << "Failed to listen to the port...\n";
        std::exit(1);
    }

    while (!end)
    {
        sockaddr_in cli{0};
        socklen_t len = sizeof(cli);

        int connfd = accept(sockfd, (sockaddr *)&cli, &len);

        if (connfd != -1)
        {
            while (!end)
            {
                status = true;
                uint8_t temp[sizeof(buffer)]{0};

                {
                    std::scoped_lock<std::mutex> lock(mtx);
                    memcpy(temp, buffer, sizeof(temp));
                }

                if (sizeof(temp) != write(connfd, temp, sizeof(temp)))
                {
                    status = false;
                    close(connfd);
                    break;
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(Setting::INTERVAL / 2));
            }
        }
    }
}
