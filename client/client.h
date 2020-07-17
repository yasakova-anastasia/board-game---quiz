#pragma once
#include <iostream>
#include <string>
#include <cstring>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <thread>
#include <vector>

#define SERVER_IP "127.0.0.1"
#define DEFAULT_PORT 1614
#define BUFFER_SIZE 4096
#define CLIENT_CLOSE_CONNECTION_SYMBOL '#'


class Client {
    int client;
  public:
    Client();
    ~Client();
    void create();
    void drop();
    int getclient();

};
