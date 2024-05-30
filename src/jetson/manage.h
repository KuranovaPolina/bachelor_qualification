/* stream.h for Jetson Nano program */
#ifndef MANAGE_H
#define MANAGE_H

#include "stream.h"

int processManage(Stream stream);

class Manager
{
private:
    Stream *pStream;  

    string filename;

    int port;
    string host = "0.0.0.0";
    string params_filename = "";

    
    int readParams(); 

public:
    Manager(Stream *new_pstream, string new_filename);

    void showParams();

    int initManager();

    int process();

};

#endif /* MANAGE_H */
