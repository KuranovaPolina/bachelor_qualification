/* main.cpp for laptop */

#include <iostream>
#include <thread>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

// #include <opencv2/opencv.hpp>   // без этого не виден format ???

#include "stream.h"

using namespace std;

#define PORT 8185;

#define HOST "192.168.0.200"

int processManage()
{
    struct sockaddr_in addr; // структура с адресом
    // struct hostent* hostinfo;
    // int port = atoi(PORT);
    int port = PORT;
    string hostAddr = HOST;
    int sockDescr = socket(AF_INET, SOCK_STREAM, 0); // создание TCP-сокета

    if (sockDescr < 0)
    {
        cout << "[ processManage ] Socket creation failed! " << endl;
        return -1;
    }

    // Указываем параметры сервера
    addr.sin_family = AF_INET; // домены Internet
    addr.sin_port = htons(port); // или любой другой порт...
    addr.sin_addr.s_addr = inet_addr(hostAddr.c_str());

    if(::connect(sockDescr, (struct sockaddr *)&addr, sizeof(addr)) < 0) // установка соединения с сервером
    {
        cout << "[ processManage ] Connection failed! " << errno << endl;

        // perror(errno);
        return -1;
    }

    cout << std::format("[ processManage ] Connection to server {} port {}! \n", hostAddr, port);

    string x = "0";
    char ax[1024];

    while (1)
    {
        /* Incorrect string value */
        cout << "menu" << endl;

        cin >> x;

        cout << " Send: " << send(sockDescr, x.c_str(), strlen(x.c_str()), 0) << endl;

        // struct sockaddr_in newAddr;
        // socklen_t addrlen = (socklen_t)sizeof(newAddr);
        // int newSockDescr = accept(sockDescr, (struct sockaddr *)&newAddr, &addrlen); 

        // int wait = 1;
        // while (wait)
        // {
        //     int bytes_read = recv(newSockDescr, ax, strlen(ax), 0);

        //     if (bytes_read > 0)
        //     {
        //         cout << " Rec: " << bytes_read << endl;
        //         wait = 0;
        //     }
        // }

        // cout << x << endl;

        // usleep(1000000);
    }

    close(sockDescr);

    return 0;

}

int main(int argc, char *argv[])
{
    std::thread manage_thread([]()
    {
        if (processManage() == -1)
        {
            return -1;
        }

        return 0;
    });

    Stream stream = Stream("params.json");

    if (stream.isOpened())
    {
        stream.showParams();

        stream.process();
    }
    else
    {
        cout << "[ main ] Stream is not opened! \n";

        return -1;
    }/*   */
    
    manage_thread.join();

    return 0;
}
