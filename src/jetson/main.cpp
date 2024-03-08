/* main.cpp for Jetson Nano program */

#include <iostream>
#include <opencv2/opencv.hpp>
#include <unistd.h>

using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
    /* Variables */
    string cam0pipline = "nvarguscamerasrc sensor-id=0 ! video/x-raw(memory:NVMM), width=(int)640, height=(int)360, framerate=(fraction)30/1 ! nvvidconv flip-method=0 ! video/x-raw, width=(int)640, height=(int)360, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";
    string cam1pipline = "nvarguscamerasrc sensor-id=1 ! video/x-raw(memory:NVMM), width=(int)640, height=(int)360, framerate=(fraction)30/1 ! nvvidconv flip-method=0 ! video/x-raw, width=(int)640, height=(int)360, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";

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