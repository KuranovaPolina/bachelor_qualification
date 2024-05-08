/* main.cpp for Jetson Nano program */

#include <iostream>
// #include <opencv2/opencv.hpp>

#include "stream.h"

int main(int argc, char *argv[])
{
    Stream stream = Stream("params.json");

    if (stream.isOpened())
    {
        stream.showParams();

        // stream.process();        
    }

    return 0;
}
