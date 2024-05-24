/* main.cpp for laptop */

#include <iostream>
#include <unistd.h>
#include <format>

#include <thread>

#include "stream.h"
#include "manage.h"

using namespace std;

void managerFunc()
{
    Manager manager = Manager("manage.json");

    if (manager.initManager() == 0) 
    {
        manager.process();        
    }    
    else
    {
        cout << "[managerFunc] Manager is not inited! \n";
    }
}

int main(int argc, char *argv[])
{
    Stream stream = Stream("params.json");

    std::thread manage_thread(managerFunc); 

    if (stream.isOpened())
    {
        stream.showParams();

        stream.process(); /*   */

        manage_thread.join();
    }
    else
    {
        cout << "[ main ] Stream is not opened! \n";

        return -1;
    } 
    
    return 0;
}
