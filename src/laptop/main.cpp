/* main.cpp for laptop */

#include <iostream>
#include <thread>
#include <unistd.h>

#include "stream.h"

int processManage()
{
    int x = 0;
    while (1)
    {

        /* Incorrect string value */
        cout << "menu" << endl;

        cin >> x;

        cout << x << endl;

        // usleep(1000000);
    }
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
    }
    
    manage_thread.join();

    return 0;
}
