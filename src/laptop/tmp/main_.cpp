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

VideoCapture cap("udpsrc name=test port=6666 ! application/x-rtp, encoding-name=H264 ! rtph264depay ! h264parse ! video/x-raw, stream-format=byte-stream ! appsink", CAP_GSTREAMER);
    
    // VideoCapture cap(0);
    // VideoCapture cap("udpsrc name=test port=6666 ! application/x-rtp, encoding-name=H264 ! rtph264depay ! avdec_h264 ! videoconvert ! video/x-raw, format=BGR ! appsink", cv::CAP_ANY);
    //  ! application/x-rtp,encoding-name=JPEG ! rtpjpegdepay ! jpegdec ! jpegenc snapshot=TRUE ! filesink location=test.jpeg");
// udpsrc address=192.168.0.200 ! rtph264depay ! x264dec

    // cout << cap.open("udpsrc name=test port=6666 ! application/x-rtp, encoding-name=H264 ! rtph264depay ! avdec_h264 ! videoconvert ! video/x-raw, format=BGR ! appsink", cv::CAP_ANY);

    while (1)
    {
cout << getBuildInformation();

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

// cap.release();

    return 0;

} 

// 


// mkdir build_opencv
// cd build_opencv

// QT5PATH=/Users/kuranovapolina/Qt/5.13.1/clang_64/

// cmake -D CMAKE_BUILD_TYPE=RELEASE \
// -D CMAKE_INSTALL_PREFIX=../opencv \
// -D OPENCV_EXTRA_MODULES_PATH=../opencv_contrib/modules \
// -D INSTALL_PYTHON_EXAMPLES=ON \
// -D INSTALL_C_EXAMPLES=ON \
// -D BUILD_opencv_python2=OFF \
// -D BUILD_opencv_python3=ONPf \
// -D OPENCV_ENABLE_NONFREE=ON \
// -D PYTHON3_EXECUTABLE=$(which python3)  \
// -D WITH_GSTREAMER=ON \
// -D WITH_OPENGL=ON


/* 

cmake -D CMAKE_BUILD_TYPE=RELEASE \
-D CMAKE_INSTALL_PREFIX=/usr/local \
-D OPENCV_EXTRA_MODULES_PATH=../opencv_contrib/modules \
-D INSTALL_PYTHON_EXAMPLES=OFF \
-D INSTALL_C_EXAMPLES=OFF \
-D BUILD_opencv_python2=OFF \
-D BUILD_opencv_python3=ONPf \
-D OPENCV_ENABLE_NONFREE=ON \
-D PYTHON3_EXECUTABLE=$(which python3)  \
-D WITH_GSTREAMER=ON \
-D WITH_FFMPEG=ON ../opencv



mkdir build
cd build

QT5PATH=/Users/kuranovapolina/Qt/6.5.1/macos/
cmake -D CMAKE_BUILD_TYPE=RELEASE \
    -D CMAKE_INSTALL_PREFIX=/usr/local \
    -D CMAKE_INSTALL_RPATH=/usr/local/lib/ \
    -D OPENCV_EXTRA_MODULES_PATH=../opencv_contrib/modules \
    -D INSTALL_PYTHON_EXAMPLES=OFF \
    -D INSTALL_C_EXAMPLES=OFF \
    -D BUILD_opencv_python2=OFF \
    -D BUILD_opencv_python3=ONPf \
    -D OPENCV_ENABLE_NONFREE=ON \
    -D PYTHON3_EXECUTABLE=$(which python3) \
    -D WITH_GSTREAMER=ON \
    -D WITH_OPENGL=ON \
    -D CMAKE_PREFIX_PATH=$QT5PATH \
    -D CMAKE_MODULE_PATH="$QT5PATH"/lib/cmake \
    -D BUILD_EXAMPLES=ON ../opencv




QT5PATH=/Users/kuranovapolina/Qt/6.5.1/macos/
cmake -D CMAKE_BUILD_TYPE=RELEASE \
    -D CMAKE_INSTALL_PREFIX=/usr/local \
    -D CMAKE_MACOSX_RPATH=1 \
    -D CMAKE_INSTALL_RPATH=${CMAKE_INSTALL_PREFIX}/lib \
    -D OPENCV_EXTRA_MODULES_PATH=../opencv_contrib/modules \
    -D INSTALL_PYTHON_EXAMPLES=OFF \
    -D INSTALL_C_EXAMPLES=OFF \
    -D BUILD_opencv_python2=OFF \
    -D BUILD_opencv_python3=ONPf \
    -D OPENCV_ENABLE_NONFREE=ON \
    -D PYTHON3_EXECUTABLE=$(which python3) \
    -D WITH_GSTREAMER=ON \
    -D WITH_OPENGL=ON \
    -D CMAKE_PREFIX_PATH=$QT5PATH \
    -D CMAKE_MODULE_PATH="$QT5PATH"/lib/cmake \
    -D BUILD_EXAMPLES=ON ../opencv   
 */
