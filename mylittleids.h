#ifndef MYLITTLEIDS_H
#define MYLITTLEIDS_H

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <cerrno>
#include <fcntl.h>
#include <ctime>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <fstream>

bool isPortOpen(int port);
int monitorPort(int port);
int Makescript(int check);
int runscript(int check);
int exiting(const char *toPrint, int socketToClose);


#endif