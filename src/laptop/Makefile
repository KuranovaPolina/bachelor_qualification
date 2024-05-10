files = main.cpp stream.cpp manage.cpp

all:
	g++ -std=c++20 -Wall -I/usr/local/Cellar/opencv/4.9.0_6/include/opencv4 $(files) -L/usr/local/lib/ -lopencv_core -lopencv_highgui -lopencv_videoio -pthread -o main
	install_name_tool -change @rpath/libopencv_core.409.dylib /usr/local/lib/libopencv_core.409.dylib main    
	install_name_tool -change @rpath/libopencv_highgui.409.dylib /usr/local/lib/libopencv_highgui.409.dylib main
	install_name_tool -change @rpath/libopencv_videoio.409.dylib /usr/local/lib/libopencv_videoio.409.dylib main
