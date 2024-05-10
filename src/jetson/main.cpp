/* main.cpp for Jetson Nano program */

#include <iostream>
#include <format>

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#include <thread>

#include "stream.h"
#include "manage.h"

int main(int argc, char *argv[])
{
    Stream stream = Stream("params.json");

    if (stream.isOpened())
    {
        stream.showParams();

        std::thread manage_thread([]()
        {
            if (processManage() == -1)
            {
                return -1;
            } 
            
            return 0;
        });

        stream.process();    
 
        manage_thread.join();           
    }
    else
    {
        cout << "[main] Stream is not opened! \n";

        return -1;
    }/*  */

    return 0;
}
