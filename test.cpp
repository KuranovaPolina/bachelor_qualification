// test.cpp
// from simple_camera.cpp

/* #include <../../../usr/local/include/opencv4/opencv2/opencv.hpp> */
#include <iostream>
#include <opencv2/opencv.hpp>

std::string gstreamer_pipeline1 (int capture_width, int capture_height, int display_width, int display_height, int framerate, int flip_method) {
    // return "nvarguscamerasrc sensor-id=1 ! video/x-raw(memory:NVMM), width=(int)" + std::to_string(capture_width) + ", height=(int)" +
    //        std::to_string(capture_height) + ", framerate=(fraction)" + std::to_string(framerate) +
    //        "/1 ! nvvidconv flip-method=" + std::to_string(flip_method) + " ! video/x-raw, width=(int)" + std::to_string(display_width) + ", height=(int)" +
    //        std::to_string(display_height) + ", format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";

    return "nvarguscamerasrc sensor-id=0 ! video/x-raw(memory:NVMM), width=(int)1280, height=(int)720, framerate=(fraction)30/1 ! nvvidconv flip-method=0 ! video/x-raw, width=(int)1280, height=(int)720, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";
}

std::string gstreamer_pipeline2 (int capture_width, int capture_height, int display_width, int display_height, int framerate, int flip_method) {
    // return "nvarguscamerasrc sensor-id=1 ! video/x-raw(memory:NVMM), width=(int)" + std::to_string(capture_width) + ", height=(int)" +
    //        std::to_string(capture_height) + ", framerate=(fraction)" + std::to_string(framerate) +
    //        "/1 ! nvvidconv flip-method=" + std::to_string(flip_method) + " ! video/x-raw, width=(int)" + std::to_string(display_width) + ", height=(int)" +
    //        std::to_string(display_height) + ", format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";

    return "nvarguscamerasrc sensor-id=1 ! video/x-raw(memory:NVMM), width=(int)1280, height=(int)720, framerate=(fraction)30/1 ! nvvidconv flip-method=0 ! video/x-raw, width=(int)1280, height=(int)720, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";
}

// gst-launch-1.0 nvarguscamerasrc ! video/x-raw, width=1280, height=720, framerate=30/1 ! nvvidconv flip-method=0 ! video/x-raw, width=1280, height=720, format=BGRx ! videoconvert ! video/x-raw, format=BGR ! appsink

int main()
{
    int capture_width = 1280 ;
    int capture_height = 720 ;
    int display_width = 1280 ;
    int display_height = 720 ;
    int framerate = 30 ;
    int flip_method = 0 ;

    std::string pipeline1 = gstreamer_pipeline1(capture_width,
	capture_height,
	display_width,
	display_height,
	framerate,
	flip_method);


    std::string pipeline2 = gstreamer_pipeline2(capture_width,
	capture_height,
	display_width,
	display_height,
	framerate,
	flip_method);
    
    std::cout << "Using pipeline: \n\t" << pipeline1 << "\n";
    std::cout << "Using pipeline: \n\t" << pipeline2 << "\n";
 
    cv::VideoCapture cap(pipeline1, cv::CAP_ANY);
    cv::VideoCapture cap2(pipeline2, cv::CAP_ANY);
    // cap.set(cv::CAP_PROP_FRAME_WIDTH , 160);
    // cap.set(cv::CAP_PROP_FRAME_HEIGHT , 120);

    if(!cap.isOpened() || !cap2.isOpened()) {
        std::cout<<"Failed to open camera."<<std::endl;
        return (-1);
    }

    cv::namedWindow("CSI Camera", cv::WINDOW_AUTOSIZE);
    cv::Mat img;
    cv::Mat img2;

    cv::Mat imgRes;

    std::cout << "Hit ESC to exit" << "\n" ;
    while(true)
    {
    	if (!cap.read(img) || !cap2.read(img2)) {
            std::cout<<"Capture read error"<<std::endl;
            break;
	    }
        else 
        {
            //  std::cout<<"123"<<std::endl;
        }
	
        cv::hconcat(img, img2, imgRes);

        cv::imshow("CSI Camera",imgRes);
        int keycode = cv::waitKey(10) & 0xff ; 
        if (keycode == 27) 
            break ;
    }

    cap.release();
    cv::destroyAllWindows() ;
    return 0;
}


