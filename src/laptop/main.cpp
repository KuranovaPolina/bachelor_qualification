// #include "opencv2/imgproc.hpp"
// // #include "opencv2/objdetect/objdetect.hpp"
// #include "opencv2/videoio/videoio.hpp"
// #include "opencv2/imgcodecs/imgcodecs.hpp"
// #include "opencv2/core/core.hpp"
// #include "opencv2/highgui.hpp"
#include <opencv2/opencv.hpp>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <time.h>

using namespace cv;
using namespace std;

int main(int argc, const char** argv)
{
    // VideoCapture cap("rtsp://127.0.0.1:8554/test");

// VideoCapture cap("udpsrc name=test port=6666 ! application/x-rtp, encoding-name=H264 ! rtph264depay ! h264parse ! video/x-raw, stream-format=byte-stream ! appsink", CAP_GSTREAMER);
    
    VideoCapture cap("udpsrc name=test port=6666 ! application/x-rtp, encoding-name=H264 ! rtph264depay ! avdec_h264 ! videoconvert ! video/x-raw, format=(string)BGR ! appsink", CAP_GSTREAMER);
    //  ! application/x-rtp,encoding-name=JPEG ! rtpjpegdepay ! jpegdec ! jpegenc snapshot=TRUE ! filesink location=test.jpeg");
// udpsrc address=192.168.0.200 ! rtph264depay ! x264dec

    while (1)
    {
        int res = cap.isOpened();
        if (!res) {
            cout << res << endl;

            break; 
        } else  {
            Mat img;
            cap >> img;
            namedWindow("Video", WINDOW_AUTOSIZE);
            imshow("Video", img);

            int key2 = waitKey(20);
            if (key2 == 27) break;
        }

    }
    return 0;

} 
