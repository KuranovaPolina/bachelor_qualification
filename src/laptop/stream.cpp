/* stream.cpp for laptop */

#include <iostream>

#include <opencv2/opencv.hpp>

#include "stream.h"

using namespace cv;
using namespace std;

int Stream::readParams(string params_filename)
{
    if (params_filename.empty() || params_filename == "")
    {
        cout << "[ Stream::readParams ] Incorrect filename with stream params! \n";

        return -1;
    }

    FileStorage fs(params_filename, FileStorage::READ);

    if (!fs.isOpened())
    {
        cout << std::format("[ Stream::readParams ] File {} is not opened! \n",
            params_filename);

        return -1;
    }
            
    port = fs["stream_params"]["port"];
    concat_type = fs["stream_params"]["concat_type"];

    fs.release();

    if (concat_type < 0 || concat_type > 1)
    {
        cout << "[ Stream::readParams ] Incorrect concatenate type! \n";

        return -1;
    }

    return 0;
}

Stream::Stream(string params_filename)
{
    cout << "[ Stream constructor ] Hello, I'm a new Stream!" << endl;

    if (readParams(params_filename) != -1)
    {
       stream_inited = true; 
    }   
}

bool Stream::isOpened()
{
    return stream_inited;
}

void Stream::showParams()
{
    cout << std::format("[ Stream::showParams ] port: {} \n\tconcat_type: {} \n", 
        to_string(port), to_string(concat_type));
}

string Stream::capture_pipeline()
{
    return std::format("udpsrc port={} ! application/x-rtp, encoding-name=H264 ! \
 rtph264depay ! avdec_h264 ! videoconvert ! video/x-raw, format=BGR ! appsink", 
        to_string(port));
}

int Stream::process()
{
    string cappipeline = capture_pipeline();

    VideoCapture cap(cappipeline, CAP_GSTREAMER);

    Mat imgRes;
    Mat img1, img2;

    Size s;

    int display_width = 0, display_height = 0;

    int start_row0 = 0, end_row0 = 0, start_col0 = 0, end_col0 = 0,
    start_row1 = 0, end_row1 = 0, start_col1 = 0, end_col1 = 0;

    cout << cappipeline << endl;

    if (!cap.isOpened()) {
        cout << "[ Stream::process ] Capture error - capture is not opened! "  << endl;

        return -1; 
    } 

    while (1)
    {
        if (!cap.read(imgRes)) 
        {
            cout << "[ Stream::process ] Capture error - capture reading error! "  << endl;

            return -1; 
        }

        // namedWindow("Video", WINDOW_AUTOSIZE);
        // imshow("Video", imgRes);

        s = imgRes.size();

        if (concat_type == 0)
        {
            display_width = s.width / 2;
            display_height = s.height;

            start_row0 = 0;
            end_row0 = display_height;

            start_row1 = 0;
            end_row1 = display_height;

            start_col0 = 0;
            end_col0 = display_width;

            start_col1 = display_width;
            end_col1 = display_width * 2;

        }
        else if (concat_type == 1)
        {
            display_width = s.width;
            display_height = s.height / 2;

            start_row0 = 0;
            end_row0 = display_height;

            start_row1 = display_height;
            end_row1 = display_height * 2;

            start_col0 = 0;
            end_col0 = display_width;

            start_col1 = 0;
            end_col1 = display_width;
        }
        else
        {
            cout << "[ Stream::process ] Incorrect concatenate type! " << endl;

            break;                 
        }

        img1 = imgRes(Range(start_row0, end_row0), Range(start_col0, end_col0));
        img2 = imgRes(Range(start_row1, end_row1), Range(start_col1, end_col1));

        namedWindow("Video1", WINDOW_AUTOSIZE);
        imshow("Video1", img1);

        namedWindow("Video2", WINDOW_AUTOSIZE);
        imshow("Video2", img2);

        int key2 = waitKey(1);
        if (key2 == 27) 
            break;

    }

    cap.release();

    cv::destroyAllWindows();

    return 0;
}
