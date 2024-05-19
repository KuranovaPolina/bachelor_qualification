/* main_base.cpp for Jetson Nano program */

#include <iostream>
#include <opencv2/opencv.hpp>
// #include <unistd.h>

using namespace std;
using namespace cv;

string capture_pipeline(int sensorId, int capture_width, int capture_height, int display_width, int display_height, int framerate, int flip_method)
{
    /* nvarguscamerasrc (libatgus GStreamer GST-nvarguscamerasrc) or v4l2*/
    return "nvarguscamerasrc sensor-id=" + to_string(sensorId) + 
    " ! video/x-raw(memory:NVMM), width=(int)" + to_string(capture_width) + 
    ", height=(int)" + to_string(capture_height) + 
    ", framerate=(fraction)" + to_string(framerate) + "/1 ! nvvidconv flip-method=" + to_string(flip_method) + 
    " ! video/x-raw, width=(int)" + to_string(display_width) + ", height=(int)" + to_string(display_height) + 
    ", format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";
                      
    // return "nvarguscamerasrc sensor-id=" + to_string(sensorId) + 
    // " ! video/x-raw(memory:NVMM), width=" + to_string(capture_width) + 
    // ", height=" + to_string(capture_height) + 
    // ", format = NV12, framerate=" + to_string(framerate) + "/1 ! nvvidconv" +
    // " ! video/x-raw, format=(string)I420 ! appsink";
}

string streaming_pipeline(int capture_width, int capture_height)
{
    /* rtsp */

    /* udp */
    // return "appsrc ! video/x-raw, format=BGR, width=640, height = 360 ! videoconvert ! x264enc ! rtph264pay ! udpsink host=192.168.0.103 port=6666 name=test";  

    // return "appsrc ! videoconvert ! videoscale ! video/x-raw, width=640, height = 360 ! x264enc ! rtspclientsink location=rtsp://192.168.0.200:6666/test";

    // appsrc ! videoconvert ! videoscale ! 

    return "appsrc is-live=true ! video/x-raw, format=BGR, width="+ to_string(capture_width * 2) +
    ", height = " + to_string(capture_height) + 
    ", stream-format=byte-stream ! videoconvert ! x264enc ! rtph264pay mtu=1400 ! udpsink host=192.168.0.103 port=6666 sync=false async=false";  

    // return "appsrc is-live=true ! video/x-raw, format=BGR, width="+ to_string(capture_width * 2) +
    // ", height = " + to_string(capture_height) + 
    // ", stream-format=byte-stream ! videoconvert ! x264enc ! rtph264pay mtu=1400 ! udpsink host=192.168.0.200 port=6666 sync=false async=false";  

}

int main(int argc, char *argv[])
{
    /* Variables */
    int framerate = 30;
    int width = 640;
    int height = 360;
    int flip_method = 2;

    string cam0pipeline = capture_pipeline(0, width, height, width, height, framerate, flip_method);
    string cam1pipeline = capture_pipeline(1, width, height, width, height, framerate, flip_method);
    string streampipeline = streaming_pipeline(width, height);

    VideoCapture cam0Capture(cam0pipeline, CAP_GSTREAMER); /* others */
    VideoCapture cam1Capture(cam1pipelinee, CAP_GSTREAMER);

    VideoWriter writer(streampipeline, 0, framerate, Size(width * 2, height), true);

    Mat img0;
    Mat img1;

    Mat imgRes;
    // Mat imgRes(Size(1280, 360), CV_8UC2);

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

/* joining by height */
        hconcat(img0, img1, imgRes);

/* joining by width */
        // vconcat(img0, img1, imgRes);

        imshow("Pair",imgRes);


    /* Packing into a stream */


    /* Video stream transfer */
        if (!writer.isOpened()) {
            cout << "Writer error" << endl;
            return (-1);
	    }

        writer.write(imgRes);

        int keycode = cv::waitKey(10)/* & 0xff*/ ; 
        if (keycode == 27) 
            break ;
    }

    /* Closing */
    cam0Capture.release();
    cam1Capture.release();

    writer.release();

    cv::destroyAllWindows();

    return 0;
} 