/* manage.cpp for laptop */

#include <iostream>
#include <unistd.h>
#include <format>

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#include "manage.h"

using namespace std;

int processManage()
{
    struct sockaddr_in addr; 
    int port = PORT;
    string hostAddr = HOST;

    int sockDescr = socket(AF_INET, SOCK_STREAM, 0); /* TCP */

    int command = 0;
    string command_buf;  
    char file_buf[4096], recbuf[1024];
    FILE *fileDescr;
    int fileReadRes, bytes_read;
    string fileName = "test.json";

    if (sockDescr < 0)
    {
        cout << "[ ERROR processManage ] Socket creation failed! " << endl;

        return -1;
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(hostAddr.c_str());   

    if(::connect(sockDescr, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        cout << "[ ERROR processManage ] Connection failed! " << endl;

        return -1;
    }

    cout << std::format("[ INFO processManage ] Connection to server {} port {}! \n", hostAddr, port);

    while (1)
    {
        cout << "Menu!\n\t1 - send 1\n\t2 - send params (test.json)" << endl;

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
            case 1:
                cout << "[ INFO processManage ] Send: " << send(sockDescr, command_buf.c_str(), strlen(command_buf.c_str()), 0) << endl;                 

                break;

            case 2:
                fileDescr = fopen(fileName.c_str(), "rb");
                if (fileDescr == NULL)
                {
                    cout << std::format("[ ERROR processManage ] Can't open file {}! ", fileName) << endl;                 
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
                }
                break;
            
            default:
                cout << "[ ERROR processManage ] Incorrect menu value! " << endl;

                break;
        }

        switch(command)
        {
            case 1: case 2:
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
                    
                break;
            
            default:
                cout << "[ processManage ] Incorrect menu value! " << endl;

                break;
        }
    }

    close(sockDescr);

    return 0;
}
