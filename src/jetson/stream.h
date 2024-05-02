/* stream.h for Jetson Nano program */
#ifndef STREAM_H
#define STREAM_H

#include <iostream>

using namespace std;

class Stream 
{
private:
/* Capture params. */
    int capture_width = 0;   /*  > 0*/
    int capture_height = 0;

    int display_width = 0;
    int display_height = 0;

    int framerate = 0;   /* [1 - 120] */

    /*
        0 - identity - no rotation (default)
        1 - counterclockwise - 90 degrees
        2 - rotate - 180 degrees
        3 - clockwise - 90 degrees
        4 - horizontal flip
        5 - upper right diagonal flip
        6 - vertical flip
        7 - upper-left diagonal
    */
    int flip_method = 0;

    int format = 0;

/* Stream params. */
    
    int mtu = 1400;
    string host = "192.168.0.101";
    int port = 6666;

    /*
        0 - horisontal
        1 - vertical
    */
    int concat_type = 0;

public:
    Stream();

    void showParams();

    string capture_pipline(int sensorId);

    string streaming_pipline();

    int process();

};

#endif  /* STREAM_H */
