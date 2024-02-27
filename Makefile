all:
	g++ -std=c++11 -Wall -I/usr/local/include/opencv4 test.cpp -L/usr/lib/aarch64-linux-gnu -lopencv_core -lopencv_highgui -lopencv_videoio -o test