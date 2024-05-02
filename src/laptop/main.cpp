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
    
// cout << getBuildInformation();

    VideoCapture cap("udpsrc port=6666 ! application/x-rtp, encoding-name=H264 ! rtph264depay ! avdec_h264 ! videoconvert ! video/x-raw, format=BGR ! appsink", CAP_GSTREAMER);
    //  ! application/x-rtp,encoding-name=JPEG ! rtpjpegdepay ! jpegdec ! jpegenc snapshot=TRUE ! filesink location=test.jpeg");
// udpsrc address=192.168.0.200 ! rtph264depay ! x264dec

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

            // cv::split(imgRes, img);
            // img1 = imgRes(Range(0, 359), Range(0, 639));
            // img2 = imgRes(Range(0, 359), Range(640, 1279));

            namedWindow("Video", WINDOW_AUTOSIZE);
            imshow("Video", imgRes);

            // namedWindow("Video1", WINDOW_AUTOSIZE);
            // imshow("Video1", img1);

            // namedWindow("Video2", WINDOW_AUTOSIZE);
            // imshow("Video2", img2);

            int key2 = waitKey(1);
            if (key2 == 27) break;
        }

    }
    return 0;

} 
