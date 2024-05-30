/* manage.cpp for laptop */

#include <iostream>
#include <unistd.h>
#include <format>

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#include <opencv2/opencv.hpp>

#include "manage.h"

using namespace std;
using namespace cv;

Manager::Manager(string new_filename)
{
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

void Manager::showParams()
{
    cout << std::format("[ Manager::showParams ] port: {} \n\thost: {} \n\tparams_filename: {} \n", 
        to_string(port), host, params_filename);
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
    struct sockaddr_in addr;

    int sockDescr = socket(AF_INET, SOCK_STREAM, 0); /* TCP */

    int command = 0;
    string command_buf;  
    char file_buf[4096], recbuf[1024];
    FILE *fileDescr;
    int fileReadRes, bytes_read;

    bool needToWait = false;

    if (sockDescr < 0)
    {
        cout << "[ ERROR processManage ] Socket creation failed! " << endl;

        return -1;
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(host.c_str());   

    if(::connect(sockDescr, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        cout << "[ ERROR processManage ] Connection failed! " << endl;

        return -1;
    }

    cout << std::format("[ INFO processManage ] Connection to server {} port {}! \n", host, port);

    while (1)
    {
        cout << std::format("Menu!\n\t1 - stop\n\t2 - start\n\t3 - restart\n\t4 - send params from file {}", params_filename)<< endl;

        cin >> command;

        if (cin.fail()) 
        { 
            cerr << "error: invalid input.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        command_buf = to_string(command);

        switch(command)
        {
            case 1: case 2: case 3:
                cout << "[ INFO processManage ] Send: " << send(sockDescr, command_buf.c_str(), strlen(command_buf.c_str()), 0) << endl;

                needToWait = true;                

                break;

            case 4:
                fileDescr = fopen(params_filename.c_str(), "rb");
                if (fileDescr == NULL)
                {
                    cout << std::format("[ ERROR processManage ] Can't open file {}! ", params_filename) << endl;                 
                }
                else
                {
                    memset(file_buf, 0, sizeof(file_buf));
                    fileReadRes = fread(file_buf, sizeof(char), sizeof(file_buf), fileDescr);

                    if (fileReadRes != 0)
                    {
                       cout << "[ INFO processManage ] Send: " << send(sockDescr, command_buf.c_str(), strlen(command_buf.c_str()), 0) << endl;
                       cout << "[ INFO processManage ] Send: " << send(sockDescr, file_buf, fileReadRes, 0) << endl;
                    }
                    else 
                    {
                        cout << "[ ERROR processManage ] Nothing to send! " << endl;                 
                    }

                    fclose(fileDescr);

                    needToWait = true; 
                }
                break;
            
            default:
                cout << "[ ERROR processManage ] Incorrect menu value! " << endl;

                break;
        }

        if (needToWait)
        {
            bytes_read = recv(sockDescr, recbuf, sizeof(recbuf), 0);

            if (bytes_read < 0)
            {
                cout << "[ ERROR processManage ] Message can't be recieved! " << endl;
            }
            else if (bytes_read == 0)
            {
                cout << "[ ERROR processManage ] Server is turned off! " << endl;
            }
            else
            {
                cout << std::format("[ INFO processManage ] Rec: {} \n", string(recbuf, 0, bytes_read));
            }
           
        }
    }

    close(sockDescr);

    return 0;
}
