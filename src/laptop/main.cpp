/* main.cpp for laptop */

#include <iostream>
#include <unistd.h>
#include <format>

#include <thread>

#include "stream.h"
#include "manage.h"

using namespace std;

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

        stream.process(); /*   */

        manage_thread.join();
    }
    else
    {
        cout << "[ main ] Stream is not opened! \n";

        return -1;
    } 
    
    return 0;
}
