#include "admin.h"

admin::admin()
{

}

void admin::create() {
    struct sockaddr_in server_address;

    client = socket(AF_INET, SOCK_STREAM, 0);
    if (client < 0) {
        std::cout << "CLIENT ERROR: establishing socket error.\n";
        exit(0);
    }

    server_address.sin_port = htons(DEFAULT_PORT);
    server_address.sin_family = AF_INET;
    inet_pton(AF_INET, SERVER_IP, &server_address.sin_addr);

    std::cout << "\n=> Client socket created.\n";

    int ret = connect(client, reinterpret_cast<const struct sockaddr*>(&server_address), sizeof(server_address));
    if (ret < 0) {
        std::cout << "CLIENT ERROR \n";
    }

}

void admin::drop() {
    close(client);
    std::cout << "\n Goodbye... " << std::endl;
}

int admin::getadmin() {
    return client;
}
