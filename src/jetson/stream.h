/* stream.h for Jetson Nano program */
#ifndef STREAM_H
#define STREAM_H

#include <iostream>

using namespace std;

class Stream 
{
private:
/* Capture params. */
    int capture_width = 0;
    int capture_height = 0;

    int display_width = 0;
    int display_height = 0;

    int framerate = 0;

    int flip_method = 0;

    int format = 0;

/* Stream params. */

public:
    Stream();

    void showParams();

    string capture_pipline(int sensorId);

    string streaming_pipline();

    int process();

};

#endif  /* STREAM_H */
