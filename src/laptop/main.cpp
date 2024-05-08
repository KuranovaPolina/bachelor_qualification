/* main.cpp for laptop */

#include <iostream>

#include "stream.h"

int main(int argc, char *argv[])
{
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

    return 0;
}
