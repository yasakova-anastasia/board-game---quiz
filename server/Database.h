#pragma once
#include <my_global.h>
#include <iostream>
#include <cstring>
#include <string>
#include <mysql.h>

#define DB_HOST "localhost"
#define DB_USER "root"
#define DP_PASS ""
#define DB_NAME "test"
#define DB_TABLE "questions"

#define BUFFER_SIZE 4096


class Database {
    char counter[BUFFER_SIZE];
  public:
    Database();
    ~Database();
    void getAmount(char* cnt) const;
    int getAmount() const;
    void addStr(const char * q, const char * a, const char * r);
    void delStr(const char * num);
    void change_counter(int i);
    void getStr(int num, char * q, char * a, char * r);
    void print_db(char * res);
};