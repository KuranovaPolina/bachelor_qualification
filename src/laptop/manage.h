/* manage.h for laptop */
#ifndef MANAGE_H
#define MANAGE_H

using namespace std;

class Manager
{
private:
    string filename;

    int port;
    string host = "0.0.0.0";
    string params_filename = "";

    
    int readParams(); 

public:
    Manager(string new_filename);

    void showParams();

    int initManager();

    int process();

};

#endif /* MANAGE_H */
