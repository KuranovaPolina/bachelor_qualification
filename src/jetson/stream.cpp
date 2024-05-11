/* stream.cpp for Jetson Nano program */

#include <iostream>
#include <opencv2/opencv.hpp>

#include "stream.h"

#define GUI

using namespace std;
using namespace cv;

int Stream::readParams()
{
    if (params_filename.empty() || params_filename == "")
    {
        cout << "[ Stream::readParams ] Incorrect filename with stream params! \n";

        return -1;
    }
//mutex file?
    FileStorage fs(params_filename, FileStorage::READ);

    string camera_modes;

    if (!fs.isOpened())
    {
        cout << std::format("[ Stream::readParams ] File {} is not opened! \n",
            params_filename);
        
        return -1;
    }
 
    camera_modes = fs["capture_params"]["camera_modes"].string();

    sensor_mode = fs["capture_params"]["sensor_mode"];
    display_width = fs["capture_params"]["display_width"];
    display_height = fs["capture_params"]["display_height"];
    /* display_framerate = fs["capture_params"]["display_framerate"]; */
    flip_method = fs["capture_params"]["flip_method"];

    mtu = fs["stream_params"]["mtu"];
    host = fs["stream_params"]["host"].string();
    port = fs["stream_params"]["port"];
    concat_type = fs["stream_params"]["concat_type"];

    fs.release();

    if (concat_type < 0 || concat_type > 1)
    {
        cout << "[ Stream::readParams ] Incorrect concatenate type ! \n";

        return -1;
    }

    if (flip_method < 0 || flip_method > 7)
    {
        cout << "[ Stream::readParams ] Incorrect flip method ! \n";

        return -1;
    }

    if (camera_modes.empty() || camera_modes == "")
    {
        cout << "[ Stream::readParams ] Incorrect filename with camera modes! \n";

        return -1;
    }

    FileStorage fs_modes(camera_modes, FileStorage::READ);

    if (!fs_modes.isOpened())
    {
        cout << std::format("[ Stream::readParams ] File {} is not opened! \n",
            camera_modes);

        return -1;
    }    

    if (fs_modes[to_string(sensor_mode)].empty())
    {
        fs_modes.release();

        cout << "[ Stream::readParams ] Incorrect sensor mode! \n";

        return -1;
    }

    capture_width = fs_modes[to_string(sensor_mode)]["width"];
    capture_height = fs_modes[to_string(sensor_mode)]["height"];
    capture_framerate = fs_modes[to_string(sensor_mode)]["framerate"];

    fs_modes.release();

    return 0;
}

Stream::Stream(string filename)
{
    cout << "[ Stream::constructor ] Hello, I'm a new Stream!" << endl;

    params_filename = filename;
}

int Stream::initStream()
{
    if (readParams() != -1)
    {
        stream_inited = true;

        return 0;
    }
    else
    {
        stream_inited = false;

        return -1;
    }
}

bool Stream::isInited()
{
    return stream_inited;
}

bool Stream::isActive()
{
    return stream_active;
}

bool Stream::isStopped()
{
    return stream_stopped;
}

void Stream::showParams()
{
    cout << std::format("[ Stream::showParams ] sensor_mode: {} \
\n\tcapture_width: {} \n\tcapture_height: {} \n\tcapture_framerate: {} \
\n\tdisplay_width: {} \n\tdisplay_height: {} \n\n\tflip_method: {} \
\n\tmtu: {} \n\thost: {} \n\tport: {} \n\tconcat_type: {}\n",
        to_string(sensor_mode), to_string(capture_width), to_string(capture_height), to_string(capture_framerate),
        to_string(display_width), to_string(display_height), to_string(flip_method),
        to_string(mtu), host, to_string(port), to_string(concat_type));

    // cout << "sensor_mode: " + to_string(sensor_mode) +
    //     "\ncapture_width: " + to_string(capture_width) +
    //     "\ncapture_height: " + to_string(capture_height) +
    //     "\ncapture_framerate: " + to_string(capture_framerate) +
    //     "\ndisplay_width: " + to_string(display_width) +
    //     "\ndisplay_height: " + to_string(display_height) +
    //     /* "\nframerate: " + to_string(display_framerate)  + */
    //     "\n\nflip_method: " + to_string(flip_method) +
    //     "\nmtu: " + to_string(mtu) +
    //     "\nhost: " + host  +
    //     "\nport: " + to_string(port) +
    //     "\nconcat_type: " + to_string(concat_type) << endl;
}

string Stream::capture_pipline(int sensorId)
{
    return std::format("nvarguscamerasrc sensor-id={} sensor-mode={} \
! video/x-raw(memory:NVMM), width={} , height={}, framerate={}/1 \
! nvvidconv flip-method={} ! video/x-raw, width={}, height={}, format=BGRx ! videoconvert ! video/x-raw, format=BGR ! appsink",
        to_string(sensorId), to_string(sensor_mode), 
        to_string(capture_width), to_string(capture_height), to_string(capture_framerate),
        to_string(flip_method), to_string(display_width), to_string(display_height));

    // return "nvarguscamerasrc sensor-id=" + to_string(sensorId) + " sensor-mode=" + to_string(sensor_mode) + /*  */
    // " ! video/x-raw(memory:NVMM), width=" + to_string(capture_width) + 
    // ", height=" + to_string(capture_height) + 
    // ", framerate=" + to_string(capture_framerate) + "/1 ! nvvidconv flip-method=" + to_string(flip_method) + 
    // // " ! nvvidconv flip-method=" + to_string(flip_method) + 
    // " ! video/x-raw, width=" + to_string(display_width) + 
    // ", height=" + to_string(display_height) + /* ", framerate=" + to_string(display_framerate) + /1*/
    // ", format=BGRx ! videoconvert ! video/x-raw, format=BGR ! appsink";

    // return "nvarguscamerasrc sensor-id=" + to_string(sensorId) + 
    // " sensor-mode = " + to_string(capture_width) + 
    // " ! video/x-raw(memory:NVMM) ! nvvidconv flip-method=" + to_string(flip_method) + 
    // " ! video/x-raw, width=" + to_string(display_width) + ", height=" + to_string(display_height) + 
    // ", format=BGRx, framerate=" + to_string(framerate) + "/1 ! videoconvert ! video/x-raw, format=BGR ! appsink";

    // return "v4l2src device=/dev/video0 ! video/x-raw, width=" + to_string(capture_width) + 
    // ", height=" + to_string(capture_height) + 
    // ", framerate=" + to_string(framerate) + "/1 ! nvvidconv ! video/x-raw(memory:NVMM), width=" + 
    // to_string(display_width) + ", height=" + to_string(display_height) + 
    // ", format=BGRx ! nvvidconv ! video/x-raw, format=BGRx ! videoconvert ! video/x-raw, format=BGR ! appsink";

}

string Stream::streaming_pipline()
{
    return std::format("appsrc is-live=true ! video/x-raw, format=BGR, stream-format=byte-stream ! \
videoconvert ! x264enc ! rtph264pay mtu={} ! udpsink host={} port={}",
        to_string(mtu), host, to_string(port));

    // return "appsrc is-live=true ! video/x-raw, format=BGR, width=" + to_string(display_width * 2) +
    // ", height = " + to_string(display_height) + 
    // ", stream-format=byte-stream ! videoconvert ! x264enc ! rtph264pay mtu=1400 ! udpsink host=192.168.0.101 port=6666";  

    // return "appsrc is-live=true ! video/x-raw, format=BGR, stream-format=byte-stream ! videoconvert ! x264enc ! rtph264pay mtu=" +
    // to_string(mtu) + " ! udpsink host=" + host + " port=" + to_string(port);  


    // return "appsrc is-live=true ! video/x-raw, format=BGR, stream-format=byte-stream ! videoconvert ! x265enc ! rtph265pay mtu=" +
    // to_string(mtu) + " ! udpsink host=" + host + " port=" + to_string(port); 

    // return "appsrc is-live=true ! video/x-raw, format=BGR, stream-format=byte-stream ! videoconvert ! x264enc ! rtph264pay mtu=" +
    // to_string(mtu) + " ! tcpclientsink host=" + host + " port=" + to_string(port);  

    // return "appsrc is-live=true ! videoconvert ! videoscale ! video/x-raw, format=I420 ! x264enc speed-preset=ultrafast bitrate=800 ! video //x-h264 ! rtmp2sink location=rtmp://192.168.0.101:6666/test";
}

int Stream::process()
{
    string cam0pipline = capture_pipline(0);
    string cam1pipline = capture_pipline(1);
    string streampipline = streaming_pipline();

    VideoCapture cam0Capture(cam0pipline, CAP_GSTREAMER); /* others */
    VideoCapture cam1Capture(cam1pipline, CAP_GSTREAMER);

    Size size;

    if (concat_type == 0)
    {
        size = Size(display_width * 2, display_height);      
    }
    else if (concat_type == 0)
    {
        size = Size(display_width, display_height * 2); 
    }
    else
    {
        cout << "[ Stream::process ] Incorrect concatenate type! " << endl;

        return (-1);        
    }

    VideoWriter writer(streampipline, CAP_GSTREAMER, 0, capture_framerate, size, true);        

    Mat img0;
    Mat img1;

    Mat imgRes;

    // cout << getBuildInformation();

    if (!cam0Capture.isOpened() || !cam1Capture.isOpened()) {
        cout << "[ Stream::process ] Failed to open cameras! " << endl;

        return (-1);
    }

#ifdef GUI
    namedWindow("Pair", WINDOW_AUTOSIZE);
#endif

    stream_active = true;    

    while(!isStopped())
    {
        if (!cam0Capture.read(img0)) {
            cout << "[ Stream::process ] Capture (0) error - capture reading error! " << endl;

            return (-1);
	    }

        if (!cam1Capture.read(img1)) {
            cout << "[ Stream::process ] Capture (1) error - capture reading error! " << endl;

            return (-1);
	    }

        if (concat_type == 0)
        {
            /* joining by height */
            hconcat(img0, img1, imgRes);
        }
        else if (concat_type == 1)
        {
            /* joining by width */
            vconcat(img0, img1, imgRes);
        }
        else
        {
            cout << "[ Stream::process ] Incorrect concatenate type! " << endl;

            return (-1);
        }

#ifdef GUI
        imshow("Pair",imgRes);
#endif

    /* Packing into a stream */


    /* Video stream transfer */
        if (!writer.isOpened()) {
            cout << "[ Stream::process ] Writer error - is not opened! " << endl;

            return (-1);
	    }

        writer.write(imgRes);

#ifdef GUI
        int keycode = cv::waitKey(1)/* & 0xff*/ ; 
        if (keycode == 27) 
            break ;
#endif
    }

    /* Closing */
    cam0Capture.release();
    cam1Capture.release();

    writer.release();

    cv::destroyAllWindows();

    stream_active = false;    

    return 0;
}

void Stream::stopStream()
{
    stream_stopped = true;

    cout << stream_stopped;
}

void Stream::startStream()
{
    stream_stopped = false;

    cout << stream_stopped;
}
