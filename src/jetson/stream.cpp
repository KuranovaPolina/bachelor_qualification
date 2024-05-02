/* stream.cpp for Jetson Nano program */

#include <iostream>
#include <opencv2/opencv.hpp>

#include "stream.h"

using namespace std;
using namespace cv;

Stream::Stream()
{
    cout << "Hello, I'm a new stream!" << endl;

    FileStorage fs("params.json", FileStorage::READ);

    capture_width = fs["stream_params"]["capture_width"];
    capture_height = fs["stream_params"]["capture_height"];

    display_width = fs["stream_params"]["display_width"];
    display_height = fs["stream_params"]["display_height"];

    framerate = fs["stream_params"]["framerate"];

    flip_method = fs["stream_params"]["flip_method"];

    format = fs["stream_params"]["format"];

    mtu = fs["stream_params"]["mtu"];
    host = fs["stream_params"]["host"].string();

    port = fs["stream_params"]["port"];

    concat_type = fs["stream_params"]["concat_type"];

    fs.release();
}

void Stream::showParams()
{
    cout << "capture_width: " + to_string(capture_width) +
        "\ncapture_height: " + to_string(capture_height) +
        "\ndisplay_width: " + to_string(display_width) +
        "\ndisplay_height: " + to_string(display_height) +
        "\nframerate: " + to_string(framerate)  +
        "\nflip_method: " + to_string(flip_method) +
        "\nformat: " + to_string(format) +
        "\nmtu: " + to_string(mtu) +
        "\nhost: " + host  +
        "\nport: " + to_string(port) +
        "\nconcat_type: " + to_string(concat_type) << endl;
}

string Stream::capture_pipline(int sensorId)
{
    return "nvarguscamerasrc sensor-id=" + to_string(sensorId) + 
    " ! video/x-raw(memory:NVMM), width=" + to_string(capture_width) + 
    ", height=" + to_string(capture_height) + 
    ", framerate=" + to_string(framerate) + "/1 ! nvvidconv flip-method=" + to_string(flip_method) + 
    " ! video/x-raw, width=" + to_string(display_width) + ", height=" + to_string(display_height) + 
    ", format=BGRx ! videoconvert ! video/x-raw, format=BGR ! appsink";

    // return "v4l2src device=/dev/video0 ! video/x-raw, width=" + to_string(capture_width) + 
    // ", height=" + to_string(capture_height) + 
    // ", framerate=" + to_string(framerate) + "/1 ! nvvidconv ! video/x-raw(memory:NVMM), width=" + 
    // to_string(display_width) + ", height=" + to_string(display_height) + 
    // ", format=BGRx ! nvvidconv ! video/x-raw, format=BGRx ! videoconvert ! video/x-raw, format=BGR ! appsink";

}

string Stream::streaming_pipline()
{
    // return "appsrc is-live=true ! video/x-raw, format=BGR, width=" + to_string(display_width * 2) +
    // ", height = " + cc + 
    // ", stream-format=byte-stream ! videoconvert ! x264enc ! rtph264pay mtu=1400 ! udpsink host=192.168.0.101 port=6666";  

    return "appsrc is-live=true ! video/x-raw, format=BGR, stream-format=byte-stream ! videoconvert ! x264enc ! rtph264pay mtu=" +
    to_string(mtu) + " ! udpsink host=" + host + " port=" + to_string(port);  


    // return "appsrc is-live=true ! videoconvert ! videoscale ! video/x-raw, format=I420 ! x264enc speed-preset=ultrafast bitrate=800 ! video/x-h264 ! rtspclientsink location=rtsp://192.168.0.101:6666/test";
}

int Stream::process()
{
    string cam0pipline = capture_pipline(0);
    string cam1pipline = capture_pipline(1);
    string streampipline = streaming_pipline();

    VideoCapture cam0Capture(cam0pipline, CAP_GSTREAMER); /* others */
    VideoCapture cam1Capture(cam1pipline, CAP_GSTREAMER);

    Size size;

    if (concat_type == 1)
    {
        size = Size(display_width * 2, display_height);      
    }
    else
    {
        size = Size(display_width, display_height * 2); 
    }

    VideoWriter writer(streampipline, CAP_GSTREAMER, 0, framerate, size, true);        

    Mat img0;
    Mat img1;

    Mat imgRes;

    cout << getBuildInformation();

    if(!cam0Capture.isOpened() || !cam1Capture.isOpened()) {
        cout << "Failed to open camera." << endl;
        return (-1);
    }

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

        if (concat_type == 1)
        {
            /* joining by height */
            hconcat(img0, img1, imgRes);
        }
        else
        {
            /* joining by width */
            vconcat(img0, img1, imgRes);
        }


        imshow("Pair",imgRes);


    /* Packing into a stream */


    /* Video stream transfer */
        if (!writer.isOpened()) {
            cout << "Writer error" << endl;
            return (-1);
	    }

        writer.write(imgRes);

        int keycode = cv::waitKey(1)/* & 0xff*/ ; 
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
