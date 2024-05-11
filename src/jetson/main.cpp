/* main.cpp for Jetson Nano program */

#include <iostream>
#include <format>

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#include <thread>

#include "stream.h"
#include "manage.h"

void managerFunc(Stream *pStream)
{
    Manager manager = Manager(pStream);

    manager.process();
}

int main(int argc, char *argv[])
{
    Stream stream = Stream("params.json");

    std::thread manage_thread(managerFunc, &stream);    

    while (1)
    {
        if (!stream.isStopped())
        {
            stream.initStream();

            if (stream.isInited())
            {
                stream.showParams();

                if (stream.process() == -1) 
                {
                    cout << "[main] Stream process return -1! \n";

                    return -1;
                }  
            }
            else
            {
                cout << "[main] Stream is not inited! \n";

                return -1;
            }/*  */    
        }
    
    }

    manage_thread.join();       

    return 0;
}
