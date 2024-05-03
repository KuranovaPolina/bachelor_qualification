/* stream.h for laptop */
#ifndef STREAM_H
#define STREAM_H

#include <iostream>

using namespace std;

class Stream
{
private:
    int port = 0;

    int concat_type = 0;

public:
    Stream();

    void showParams();

    string capture_pipline();

    int process();
};

#endif /* STREAM_H */
