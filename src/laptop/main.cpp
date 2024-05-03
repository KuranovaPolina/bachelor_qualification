/* main.cpp for laptop */

#include <iostream>

#include "stream.h"

int main(int argc, char *argv[])
{
    Stream stream = Stream();

    stream.showParams();

    stream.process();

    return 0;
}
