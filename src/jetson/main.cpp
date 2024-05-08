/* main.cpp for Jetson Nano program */

#include <iostream>
#include <thread>
// #include <opencv2/opencv.hpp>

#include "stream.h"

int processManage()
{
    int x = 0;

    while(1)
    {
        cout << "menu" << endl;

        cin >> x;

        cout << x << endl;
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
        cout << "[main] Stream is not opened! \n";

        return -1;
    }

    manage_thread.join();

    return 0;
}
