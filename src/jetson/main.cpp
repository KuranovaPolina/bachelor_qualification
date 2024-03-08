/* main.cpp for Jetson Nano program */

#include <iostream>
#include <opencv2/opencv.hpp>
#include <unistd.h>

using namespace std;
using namespace cv;

string gstreamer_pipline(int sensorId, int capture_width, int capture_height, int display_width, int display_height, int framerate, int flip_method)
{
    /* nvarguscamerasrc (libatgus GStreamer GST-nvarguscamerasrc) or v4l2*/
    return "nvarguscamerasrc sensor-id=" + to_string(sensorId) + 
    " ! video/x-raw(memory:NVMM), width=(int)" + to_string(capture_width) + 
    ", height=(int)" + to_string(capture_height) + 
    ", framerate=(fraction)" + to_string(framerate) + "/1 ! nvvidconv flip-method=" + to_string(flip_method) + 
    " ! video/x-raw, width=(int)" + to_string(display_width) + ", height=(int)" + to_string(display_height) + 
    ", format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";
}

int main(int argc, char *argv[])
{
    /* Variables */
    string cam0pipline = gstreamer_pipline(0, 640, 360, 640, 360, 30, 0);
    string cam1pipline = gstreamer_pipline(1, 640, 360, 640, 360, 30, 0);

    VideoCapture cam0Capture(cam0pipline, CAP_ANY);
    VideoCapture cam1Capture(cam1pipline, CAP_ANY);

    Mat img0;
    Mat img1;

    Mat imgRes;

    /* Capture data from cameras */
    if(!cam0Capture.isOpened() || !cam1Capture.isOpened()) {
        cout << "Failed to open camera." << endl;
        return (-1);
    }

#ifdef ONLYCATURING
    namedWindow("0 CSI Camera", WINDOW_AUTOSIZE);
    namedWindow("1 CSI Camera", WINDOW_AUTOSIZE);

    while(1)
    {
        if (!cam0Capture.read(img0)) {
            cout << "Capture 0 ead error" << endl;
            return (-1);
	    }

        if (!cam1Capture.read(img1)) {
            cout << "Capture 1 read error" << endl;
            return (-1);
	    }

        imshow("0 CSI Camera", img0);
        imshow("1 CSI Camera", img1);

        int keycode = cv::waitKey(10)/* & 0xff*/ ; 
        if (keycode == 27) 
            break ;

    }
#endif

    /* Joining data */
    namedWindow("Pair", WINDOW_AUTOSIZE);

    while(1)
    {
        if (!cam0Capture.read(img0)) {
            cout << "Capture 0 ead error" << endl;
            return (-1);
	    }

        if (!cam1Capture.read(img1)) {
            cout << "Capture 1 read error" << endl;
            return (-1);
	    }

        hconcat(img0, img1, imgRes);

        imshow("Pair",imgRes);

        int keycode = cv::waitKey(10)/* & 0xff*/ ; 
        if (keycode == 27) 
            break ;
    }

    /* Packing into a stream */


    /* Video stream transfer */


    /* Closing */
    cam0Capture.release();
    cam1Capture.release();
    cv::destroyAllWindows();

    return 0;
} 