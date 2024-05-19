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
    string params_filename;

    bool stream_inited = false;

    bool stream_stopped = false;

    bool stream_active = false;

    int sensor_mode = 0;

    int capture_width = 0;   /*  > 0*/
    int capture_height = 0;
    int capture_framerate = 0;

    int display_width = 0;
    int display_height = 0;

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
    int flip_method0 = 0;  //2
    int flip_method1 = 0;  //2

    int left0 = 0;  //2
    int right0 = 0;  //2
    int top0 = 0;  //2
    int bottom0 = 0;  //2

    int left1 = 0;  //2
    int right1 = 0;  //2
    int top1 = 0;  //2
    int bottom1 = 0;  //2

/* Stream params. */
    int mtu = 0;  /* guint value range.  1500 bytes is the maximum mtu size. by experiment min 28 (8 udp header + 20 ip header) */
    string host = "0.0.0.0";
    int port = 0;

    /*
        0 - horisontal, by height
        1 - vertical, by width
    */
    int concat_type = 0;

    int readParams();      

public:
    Stream(string filename);

    int initStream();

    void showParams();

    string capture_pipline(int sensorId);

    string streaming_pipline();

    int process();

    bool isInited();

    bool isActive();

    bool isStopped();

    void stopStream();

    void startStream();
};

#endif  /* STREAM_H */
