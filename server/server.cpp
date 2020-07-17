#include <iostream>
#include <string>
#include <cstring>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <vector>
#include <thread>
#include <cstdlib>
#include <random>
#include "Database.h"

#define DEFAULT_PORT 1614
#define BUFFER_SIZE 4096
#define CLIENT_CLOSE_CONNECTION_SYMBOL '#'

int server[100];
int queue[50];
int counter = 0;


void ClientHandler(int index) {
    Database db;
    char buffer[BUFFER_SIZE];
    char msg[BUFFER_SIZE];
    int cnt = 0;
    bool flag = 0;
    int pre_flag = 0;
    std::vector <int> lib(db.getAmount());
    for (int i = 0; i < db.getAmount(); i++) {
        lib[i] = i + 1;
    }
    char q[BUFFER_SIZE];
    char a[BUFFER_SIZE];
    char r[BUFFER_SIZE];
    int num_pair = index / 3;
    int i = index % 3;

    std::default_random_engine random;
    random.seed(static_cast<unsigned int>(std::time(0)));

    while (true) {
        buffer[0] = 0;
        recv(server[index], buffer, sizeof(buffer), NULL);
        msg[0] = 0;
        switch(flag) {
            case false:
            {
                switch(buffer[0]) {
                    case '#':
                    {
                        break;
                    }
                    case '=':
                    {
                        int amount = (i == 0) ? server[index + 2] : server[index + 1];
                        if (amount == 2) {
                            strcat(msg, "$");
                            if (queue[num_pair] != 1 && queue[num_pair] != 2) {
                                queue[num_pair] = i + 1;
                                strcat(msg, "0");
                            }

                            send(server[index], msg, sizeof(msg), NULL);
                        } else {
                            strcat(msg, "!");
                            send(server[index], msg, sizeof(msg), NULL);

                            while (amount != 2) {
                                amount = (i == 0) ? server[index + 2] : server[index + 1];
                            }
                            msg[0] = 0;
                            strcat(msg, "$");
                            if (queue[num_pair] != 1 && queue[num_pair] != 2) {
                                queue[num_pair] = i + 1;
                                strcat(msg, "0");
                            }
                            send(server[index], msg, sizeof(msg), NULL);
                        }
                        break;
                    }
                    case '@':
                    {
                        if (i + 1 != queue[num_pair]) {
                            while (i + 1 != queue[num_pair]) {}
                        }
                        q[0] = 0;
                        a[0] = 0;
                        r[0] = 0;
                        int rand = random() % lib.size();
                        db.getStr(lib[rand], q, a, r);
                        lib.erase(lib.begin() + rand);
                        strcat(msg, "$");
                        strcat(msg, q);
                        strcat(msg, "$");
                        strcat(msg, a);

                        send(server[index], msg, sizeof(msg), NULL);
                        break;
                    }
                    case '^':
                    {
                        if (buffer[1] == '1' || buffer[1] == '2' || buffer[1] == '3' || buffer[1] == '4') {
                            char res[BUFFER_SIZE];
                            res[0] = 0;
                            strcat(msg, "$");
                            if (r[0] == buffer[1]) {
                                strcat(msg, "1Правильно!");
                                strcat(res, "!1\n");
                            } else {
                                strcat(msg, "0Неправильно!$");
                                int right = r[0] - 48;
                                int f_ = 1;
                                char msg_r[BUFFER_SIZE];
                                int msg_r_s = 0;
                                for (int i = 0; i < strlen(a); ++i) {
                                    if (a[i] != '$') {
                                        if (f_ == right) {
                                            msg_r[msg_r_s] = a[i];
                                            msg_r_s++;
                                        }
                                    } else {
                                        f_++;
                                    }
                                }
                                msg_r[msg_r_s] = 0;
                                strcat(msg, msg_r);
                                strcat(res, "!0\n");
                            }
                            send(server[index], msg, sizeof(msg), NULL);
                            cnt++;
                            if (cnt == 4) {
                                cnt = 0;
                            }
                        
                            int start = (i == 0) ? index : index - 1;
                            int end = start + server[start + 2];
                            for (int i = start; i < end; ++i) {
                                if (server[index] == server[i]) {
                                    continue;
                                }
                                send(server[i], res, sizeof(res), NULL);
                            }
                            queue[num_pair] = ((i == 0) ? 1 : 0) + 1;
                        }
                        break;
                    }
                    case '~':
                    {
                        db.print_db(msg);
                        send(server[index], msg, sizeof(msg), NULL);
                        flag = true;
                        break;
                    }
                    default:
                        break;
                }
                break;
            }
            case true:
            {
                switch(buffer[0]) {
                    case '#':
                    {
                        send(server[index], "#", sizeof(msg), NULL);
                        break;
                    }
                    case '+':
                    {
                        char q[BUFFER_SIZE], a[BUFFER_SIZE], r[BUFFER_SIZE];
                        q[0] = 0;
                        a[0] = 0;
                        r[0] = 0;

                        int j = 0;
                        int q_s = 0, a_s = 0, r_s = 0;
                        for (int i = 1; i < strlen(buffer); ++i) {
                            if (buffer[i] != '%') {
                                if (j == 0) {
                                    q[q_s] = buffer[i];
                                    q_s++;
                                } else if (j == 1) {
                                    a[a_s] = buffer[i];
                                    a_s++;
                                } else if (j == 2) {
                                    r[r_s] = buffer[i];
                                    r_s++;
                                }
                            } else {
                                j++;
                            }
                        }
                        q[q_s] = 0;
                        a[a_s] = 0;
                        r[r_s] = 0;
                        db.addStr(q, a, r);
                        db.print_db(msg);
                        send(server[index], msg, sizeof(msg), NULL);
                        break;
                    }
                    case '-':
                    {
                        char tmp[BUFFER_SIZE];
                        tmp[0] = 0;
                        int i = 1;
                        for (i; i < strlen(buffer); ++i) {
                            tmp[i - 1] = buffer[i];
                        }
                        tmp[i - 1] = 0;
                        db.delStr(tmp);
                        db.print_db(msg);
                        send(server[index], msg, sizeof(msg), NULL);
                        break;
                    }
                }
                break;
            }
            default:
                break;
        }
    }
}

int main(int argc, char const* argv[]) {



    int client;

    struct sockaddr_in server_address;

    client = socket(AF_INET, SOCK_STREAM, 0);

    if (client < 0) {
        std::cout << "SERVER ERROR: establishing socket error.\n";
        exit(0);
    }

    std::cout << "SERVER: Socket for server was successfully created\n";

    server_address.sin_port = htons(DEFAULT_PORT);
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htons(INADDR_ANY);

    int ret = bind(client, reinterpret_cast<struct sockaddr*>(&server_address), sizeof(server_address));

    if (ret < 0) {
        std::cout << "SERVER ERROR: binding connection. Socket has already been establishing.\n";
        return -1;
    }

    socklen_t size = sizeof(server_address);
    std::cout << "SERVER: Listening clients...\n";
    listen(client, 1);

    std::vector<std::thread> thr;
    //std::vector<int> server_;

    int i = 0;
    while (i < 100) {
        server[i] = accept(client, reinterpret_cast<struct sockaddr*>(&server_address), &size);
        if (server[i] < 0) {
            std::cout << "SERVER ERROR: Can't accepting client.\n";
        }
        if (server[i] > 0) {
            std::cout << "OK " << i <<std::endl;
            //server_.push_back(server);
            thr.push_back(std::thread(ClientHandler, i));
            if (i % 3 == 1) {
                i++;
                server[i] = 2;
                queue[counter] = 0;
                counter++;
            } else if (i % 3 == 0) {
                server[i + 2] = 1;
            }
        }
        i++;
    }

    for (int i = 0; i < thr.size(); ++i) {
        thr[i].join();
    }

    return 0;
}