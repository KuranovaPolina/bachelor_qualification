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
    flip_method0 = fs["capture_params"]["flip_method0"];
    flip_method1 = fs["capture_params"]["flip_method1"];

    left0 = fs["capture_params"]["left0"].empty() ? 0 : fs["capture_params"]["left0"];
    right0 = fs["capture_params"]["right0"].empty() ? display_width : fs["capture_params"]["right0"];
    top0 = fs["capture_params"]["top0"].empty() ? 0 : fs["capture_params"]["top0"];
    bottom0 = fs["capture_params"]["bottom0"].empty() ? display_height : fs["capture_params"]["bottom0"];

    left1 = fs["capture_params"]["left1"].empty() ? 0 : fs["capture_params"]["left1"];
    right1 = fs["capture_params"]["right1"].empty() ? display_width : fs["capture_params"]["right1"];
    top1 = fs["capture_params"]["top1"].empty() ? 0 : fs["capture_params"]["top1"];
    bottom1 = fs["capture_params"]["bottom1"].empty() ? display_height : fs["capture_params"]["bottom1"];

    concat_type = fs["capture_params"]["concat_type"];

    mtu = fs["stream_params"]["mtu"];
    host = fs["stream_params"]["host"].string();
    port = fs["stream_params"]["port"];

    fs.release();

    if (concat_type < 0 || concat_type > 1)
    {
        cout << "[ Stream::readParams ] Incorrect concatenate type ! \n";

        return -1;
    }

    if (flip_method0 < 0 || flip_method0 > 7 || flip_method1 < 0 || flip_method1 > 7)
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

    if (left0 < 0 || right0 > capture_width || top0 < 0 || bottom0 > capture_height || 
        left1 < 0 || right1 > capture_width || top1 < 0 || bottom1 > capture_height || 
        left0 >= right0 || top0 >= bottom0 || left1 >= right1 || top1 >= bottom1)
    {
        cout << "[ Stream::readParams ] Incorrect crop params ! \n";

        return -1;        
    }

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
\n\tdisplay_width: {} \n\tdisplay_height: {} \n\tflip_method0: {} \n\tflip_method1: {} \
\n\tleft0: {} \n\tright0: {} \n\ttop0: {} \n\tbottom0: {} \
\n\tleft1: {} \n\tright1: {} \n\ttop1: {} \n\tbottom1: {} \
\n\n\tmtu: {} \n\thost: {} \n\tport: {} \n\tconcat_type: {}\n",
        to_string(sensor_mode), to_string(capture_width), to_string(capture_height), to_string(capture_framerate),
        to_string(display_width), to_string(display_height), to_string(flip_method0), to_string(flip_method1),
        to_string(left0), to_string(right0), to_string(top0), to_string(bottom0),
        to_string(left1), to_string(right1), to_string(top1), to_string(bottom1), 
        to_string(mtu), host, to_string(port), to_string(concat_type));
}

string Stream::capture_pipeline(int sensorId)
{
    if (sensorId == 0)
    {
        return std::format("nvarguscamerasrc sensor-id=0 sensor-mode={} \
! video/x-raw(memory:NVMM), width={} , height={}, framerate={}/1 \
! nvvidconv flip-method={} left={} right={} top={} bottom={} ! video/x-raw, width={}, height={}, format=BGRx ! videoconvert ! video/x-raw, format=BGR ! appsink",
        to_string(sensor_mode), to_string(capture_width), to_string(capture_height), to_string(capture_framerate),
        to_string(flip_method0), to_string(left0), to_string(right0), to_string(top0), to_string(bottom0),
        to_string(display_width), to_string(display_height));
    }
    else if (sensorId == 1)
    {
        return std::format("nvarguscamerasrc sensor-id=1 sensor-mode={} \
! video/x-raw(memory:NVMM), width={} , height={}, framerate={}/1 \
! nvvidconv flip-method={} left={} right={} top={} bottom={} ! video/x-raw, width={}, height={}, format=BGRx ! videoconvert ! video/x-raw, format=BGR ! appsink",
        to_string(sensor_mode), to_string(capture_width), to_string(capture_height), to_string(capture_framerate),
        to_string(flip_method1), to_string(left1), to_string(right1), to_string(top1), to_string(bottom1), 
        to_string(display_width), to_string(display_height));
    }

}

string Stream::streaming_pipeline()
{
    return std::format("appsrc is-live=true ! video/x-raw, format=BGR, stream-format=byte-stream ! \
videoconvert ! x264enc ! rtph264pay mtu={} ! udpsink host={} port={}",
        to_string(mtu), host, to_string(port));
}

int Stream::process()
{
    string cam0pipeline = capture_pipeline(0);
    string cam1pipeline = capture_pipeline(1);
    string streampipeline = streaming_pipeline();

    cout <<cam0pipeline<< endl;

    VideoCapture cam0Capture(cam0pipeline, CAP_GSTREAMER); /* others */
    VideoCapture cam1Capture(cam1pipeline, CAP_GSTREAMER);

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
          
    VideoWriter writer(streampipeline, CAP_GSTREAMER, 0, capture_framerate, size, true);   

    cout <<streampipeline<< endl;

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


// #ifdef GUI
        int keycode = cv::waitKey(1)/* & 0xff*/ ; 
        if (keycode == 27) 
            // break ;
            stream_stopped = true;
// #endif
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
