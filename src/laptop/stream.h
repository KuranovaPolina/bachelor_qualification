/* stream.h for laptop */
#ifndef STREAM_H
#define STREAM_H

#include <iostream>

#include <opencv2/opencv.hpp>

using namespace std;

class Stream
{
private:
    string params_filename;

    bool stream_inited = false;

    int port = 0;

    int concat_type = 0;

    int readParams();

public:
    Stream(string filename);

    void showParams();

    string capture_pipeline();

    int process();

    bool isInited();

    int initStream();
};

#endif /* STREAM_H */
