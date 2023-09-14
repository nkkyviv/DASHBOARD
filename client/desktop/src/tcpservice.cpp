#include "tcpservice.h"
#include "setting.h"
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <QDebug>

void TCPService ::run(void)
{

    while (!end)
    {
        sockaddr_in servaddr{0};
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(Setting::TCP::PORT);
        servaddr.sin_addr.s_addr = inet_addr(Setting::TCP::SERVER);

        sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);

        if (sockfd != -1)
        {
            if (0 == connect(sockfd, (sockaddr *)&servaddr, sizeof(servaddr)))
            {
                while (!end)
                {
                    status = true;
                    uint8_t temp[sizeof(buffer)]{0};

                    if (sizeof(temp) == read(sockfd, temp, sizeof(temp)))
                    {
                        std::scoped_lock<std::mutex> lock(mtx);
                        memcpy(buffer, temp, sizeof(buffer));
                    }
                    else
                    {
                        status = false;
                        close(sockfd);
                        break;
                    }
                }
            }
            close(sockfd);
            std::this_thread::sleep_for(std::chrono::milliseconds(Setting::INTERVAL));
        }
    }
}