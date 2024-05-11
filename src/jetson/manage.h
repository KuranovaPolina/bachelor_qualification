/* stream.h for Jetson Nano program */
#ifndef MANAGE_H
#define MANAGE_H

#include "stream.h"

#define PORT 8185;

#define HOST "0.0.0.0"

int processManage(Stream stream);

class Manager
{
private:
    Stream *pStream;  

public:
    Manager(Stream *new_pstream);

    int process();

};

#endif /* MANAGE_H */
