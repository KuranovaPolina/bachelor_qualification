/* stream.h for laptop */
#ifndef STREAM_H
#define STREAM_H

#include <iostream>

#include <opencv2/opencv.hpp>

using namespace std;

class Stream
{
private:
    bool stream_inited = false;

    int port = 0;

    int concat_type = 0;

    int readParams(string params_filename);

public:
    Stream(string params_filename);

    void showParams();

    string capture_pipline();

    int process();

    bool isOpened();
};

#endif /* STREAM_H */
