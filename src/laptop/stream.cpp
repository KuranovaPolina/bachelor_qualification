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
    concat_type = fs["stream_params"]["concat_type"];

    fs.release();
}

void Stream::showParams()
{
    cout << "port: " + to_string(port) +
    "\nconcat_type: " + to_string(concat_type) << endl;
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

    Size s;

    int display_width = 0, display_height = 0;

    int start_row0 = 0, end_row0 = 0, start_col0 = 0, end_col0 = 0,
    start_row1 = 0, end_row1 = 0, start_col1 = 0, end_col1 = 0;

    while (1)
    {
        int res = cap.isOpened();
        if (!res) {
            cout << res << endl;

            return -1; 
        } else  {
            cap >> imgRes;

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
                cout << "wrong concat_type" << endl;

                break;                 
            }

            img1 = imgRes(Range(start_row0, end_row0), Range(start_col0, end_col0));
            img2 = imgRes(Range(start_row1, end_row1), Range(start_col1, end_col1));

            namedWindow("Video1", WINDOW_AUTOSIZE);
            imshow("Video1", img1);

            namedWindow("Video2", WINDOW_AUTOSIZE);
            imshow("Video2", img2);

            int key2 = waitKey(1);
            if (key2 == 27) break;
        }

    }

    cap.release();

    cv::destroyAllWindows();

    return 0;
}
