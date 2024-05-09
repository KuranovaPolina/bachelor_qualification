/* stream.h for Jetson Nano program */
#ifndef STREAM_H
#define STREAM_H

#include <iostream>

#include <opencv2/opencv.hpp>

using namespace std;

class Stream 
{
private:
/* Capture params. */
    bool stream_inited = false;

    int sensor_mode = 0;

    int capture_width = 0;   /*  > 0*/
    int capture_height = 0;
    int capture_framerate = 0;

    int display_width = 0;
    int display_height = 0;

    /* int display_framerate = 0; */  /* fps [1 - 120] */

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

    // int left0

/* Stream params. */
    
    int mtu = 1400;  /* guint value range.  1500 bytes is the maximum mtu size. by experiment min 28 (8 udp header + 20 ip header) */
    string host = "192.168.0.101";
    int port = 6666;

    /*
        0 - horisontal, by height
        1 - vertical, by width
    */
    int concat_type = 0;

    int readParams(string params_filename);

public:
    Stream(string params_filename);

    void showParams();

    string capture_pipline(int sensorId);

    string streaming_pipline();

    int process();

    bool isOpened();

};

#endif  /* STREAM_H */
