/* stream.cpp for laptop */

#include <iostream>
#include <opencv2/opencv.hpp>

#include "stream.h"

using namespace cv;
using namespace std;

Stream::Stream()
{
    FileStorage fs("params.json", FileStorage::READ);

    port = fs["stream_params"]["port"];

    fs.release();
}

void Stream::showParams()
{
    cout << "port: " + to_string(port) << endl;
}

string Stream::capture_pipline()
{
    return "udpsrc port=" + to_string(port) + 
    " ! application/x-rtp, encoding-name=H264 ! rtph264depay ! avdec_h264 ! videoconvert ! video/x-raw, format=BGR ! appsink";
}

int Stream::process()
{
    string capPipline = capture_pipline();

    VideoCapture cap(capPipline, CAP_GSTREAMER);

    Mat imgRes;
    Mat img1, img2;

    while (1)
    {
        int res = cap.isOpened();
        if (!res) {
            cout << res << endl;

            break; 
        } else  {
            cap >> imgRes;

            namedWindow("Video", WINDOW_AUTOSIZE);
            imshow("Video", imgRes);

            int key2 = waitKey(1);
            if (key2 == 27) break;
        }

    }

    return 0;
}
