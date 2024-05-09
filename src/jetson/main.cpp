/* main.cpp for Jetson Nano program */

#include <iostream>
#include <thread>
// #include <opencv2/opencv.hpp>

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#include "stream.h"

#define PORT 8185;

// ??????
#define HOST "0.0.0.0"

int processManage()
{
    struct sockaddr_in addr;

    int port = PORT;
    string hostAddr = HOST;

    int sockDescr = socket(AF_INET, SOCK_STREAM, 0);

    if (sockDescr < 0)
    {
        cout << "[ processManage ] Socket creation failed! " << endl;

        return -1;
    }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(hostAddr.c_str());
    addr.sin_port = htons(port);

    if (::bind(sockDescr, (struct sockaddr *)&addr, sizeof(addr)) == -1)
    {
        cout << "[ processManage ] Bind failed! " << endl;

        return -1;
    }

    cout << std::format("[ processManage ] Listener {} on port {}! ", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port)) << endl;

    if (listen(sockDescr, 5) == -1)
    {
        cout << "[ processManage ] Listening failed! " << endl;

        return -1;        
    }

    /* while (1) */
    {
        int newSockDescr;
        struct sockaddr_in newAddr;
        socklen_t addrlen = (socklen_t)sizeof(newAddr);
        int bytes_read;

        char x[1024];

        if ((newSockDescr = ::accept(sockDescr, (struct sockaddr *)&newAddr, &addrlen)) == -1)
        {
            cout << "[ processManage ] Acception failed! " << endl;

            return -1;              
        }

        cout << std::format("[ processManage ] New connection created: hostname: {}, port: {} \n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));

        while (1)
        {
            bytes_read = recv(newSockDescr, x, sizeof(x), 0);

            if (bytes_read > 0)
            {
                cout << "Rec:" << bytes_read << endl;

                // cout << "Send:" << send(newSockDescr, x, sizeof(x), 0) << endl;
            }
        }

        close(newSockDescr);
    }

    close(sockDescr);

    return 0;

    // int x = 0;
//
    // while(1)
    // {
    //     cout << "menu" << endl;
//
    //     cin >> x;
//
    //     cout << x << endl;
    // }
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
        cout << "[main] Stream is not opened! \n";

        return -1;
    }/*  */

    manage_thread.join();

    return 0;
}
