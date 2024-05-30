#include <iostream>
#include <format>
#include <unistd.h>

#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#include <opencv2/opencv.hpp>

#include "manage.h"
#include "stream.h"

using namespace std;
using namespace cv;

Manager::Manager(Stream *new_pstream, string new_filename)
{
    pStream = new_pstream;

    filename = new_filename;
}

int Manager::readParams()
{
    if (filename.empty() || filename == "")
    {
        cout << "[ Manager::readParams ] Incorrect filename with stream params! \n";

        return -1;
    }
//mutex file?
    FileStorage fs(filename, FileStorage::READ);

    if (!fs.isOpened())
    {
        cout << std::format("[ Stream::readParams ] File {} is not opened! \n",
            params_filename);
        
        return -1;
    }
 
    params_filename = fs["manage_params"]["params_filename"].string();
    host = fs["manage_params"]["host"].string();
    port = fs["manage_params"]["port"];

    fs.release();

    return 0;
}

int Manager::initManager()
{
    if (readParams() != -1)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

int Manager::process()
{
    struct sockaddr_in addr, newAddr;

    int sockDescr = socket(AF_INET, SOCK_STREAM, 0), newSockDescr;

    socklen_t newAddrlen = (socklen_t)sizeof(newAddr);
    int bytes_read, file_bytes_read, bytes_send;

    char command_buf[16], file_buf[4096];

    FILE *fileDesc;

    string answer_buf;

    if (sockDescr < 0)
    {
        cout << "[ ERROR processManage ] Socket creation failed! " << endl;

        return -1;
    }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(host.c_str());
    addr.sin_port = htons(port);

    if (::bind(sockDescr, (struct sockaddr *)&addr, sizeof(addr)) == -1)
    {
        cout << "[ ERROR processManage ] Bind failed! " << endl;

        return -1;
    }

    cout << std::format("[ INFO processManage ] Listener {} on port {}! ", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port)) << endl;

    if (listen(sockDescr, SOMAXCONN) == -1)
    {
        cout << "[ ERROR processManage ] Listening failed! " << endl;

        return -1;        
    }

    while (1 /* !pStream->finalStopStream() */) 
    {
        if ((newSockDescr = ::accept(sockDescr, (struct sockaddr *)&newAddr, &newAddrlen)) == -1)
        {
            cout << "[ ERROR processManage ] Acception failed! " << endl;

            return -1;              
        }

        cout << std::format("[ INFO processManage ] New connection created: hostname: {}, port: {} \n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));

        while (1)
        {
            bytes_read = recv(newSockDescr, command_buf, sizeof(command_buf), 0);

            if (bytes_read < 0)
            {
                cout << "[ ERROR processManage ] Can't read! " << endl;

                return -1; 
            }
            else if (bytes_read == 0)
            {
                cout << "[ INFO processManage ] Client turned off! " << endl;

                break; 
            }

            cout << std::format("[ processManage ] Rec: {} bytes \n", bytes_read);

            switch (atoi(command_buf))
            {
                case 1:
                    cout << "[ processManage ] get 1! " << endl;

                    pStream->stopStream();

                    while (1)
                    {
                        if (!pStream->isActive())
                        {
                            break;
                        }
                    }

                    answer_buf = to_string(pStream->isActive());

                    bytes_send = send(newSockDescr, answer_buf.c_str(), 
                        sizeof(answer_buf.c_str()), 0);

                    if (bytes_send <= 0)
                    {
                        cout << "[ ERROR processManage ] Can't send! " << endl;

                        return -1; 
                    }

                    cout << std::format("[ processManage ] Send: {} bytes \n", bytes_send);

                    break;

                case 2:
                    cout << "[ processManage ] get 2! " << endl;

                    pStream->startStream();

                    while (1)
                    {
                        if (pStream->isActive())
                        {
                            break;
                        }
                    }

                    answer_buf = to_string(pStream->isActive());

                    bytes_send = send(newSockDescr, answer_buf.c_str(), 
                        sizeof(answer_buf.c_str()), 0);

                    if (bytes_send <= 0)
                    {
                        cout << "[ ERROR processManage ] Can't send! " << endl;

                        return -1; 
                    }

                    cout << std::format("[ processManage ] Send: {} bytes \n", bytes_send);

                    break;

                case 3:
                    cout << "[ processManage ] get 3! " << endl;

                    pStream->stopStream();

                    while (1)
                    {
                        if (!pStream->isActive())
                        {
                            break;
                        }
                    }

                    pStream->startStream();  

                    while (1)
                    {
                        if (pStream->isActive())
                        {
                            break;
                        }
                    }

                    answer_buf = to_string(pStream->isActive());

                    bytes_send = send(newSockDescr, answer_buf.c_str(), 
                        sizeof(answer_buf.c_str()), 0);

                    if (bytes_send <= 0)
                    {
                        cout << "[ ERROR processManage ] Can't send! " << endl;

                        return -1; 
                    }

                    cout << std::format("[ processManage ] Send: {} bytes \n", bytes_send);

                    break;

                case 4:
                    cout << "[ processManage ] get 4! Waiting a file! " << endl;

                    /* while(1) */
                    {
                        memset(file_buf, 0, sizeof(file_buf));

                        file_bytes_read = recv(newSockDescr, file_buf, sizeof(file_buf), 0);
                        if (file_bytes_read > 0)
                        {
                            cout << std::format("[ processManage ] Rec: {} bytes \n", file_bytes_read);
//mutex file?
                            fileDesc = fopen(params_filename.c_str(), "w");

                            if (fileDesc == NULL)
                            {
                                cout << "[ ERROR processManage ] Can't open a file! " << endl;
                            }
                            else
                            {
                                fprintf(fileDesc, "%s", file_buf);

                                cout << std::format("[ INFO processManage ] {} updated! ", params_filename) << endl;

                                fclose(fileDesc);
                            }
                        }
                    }

                    bytes_send = send(newSockDescr, command_buf, sizeof(command_buf), 0);

                    if (bytes_send <= 0)
                    {
                        cout << "[ ERROR processManage ] Can't send! " << endl;

                        return -1; 
                    }

                    cout << std::format("[ processManage ] Send: {} bytes \n", bytes_send);

                    break;
                
                default:
                    cout << "[ processManage ] Wrong value! " << endl;

                    break;
            }

        }

        close(newSockDescr);
    }

    close(sockDescr);

    return 0;
}
