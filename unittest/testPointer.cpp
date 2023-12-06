#include "test.hpp"

void changePointer(Client *client) {
    Client *tmp = client;
    client = new Client();
    delete tmp;
    std::cout << "Address of client after assignment: " << client << std::endl;
}

void changeFd(Client *client, int i) {
    if (i == 0)
        changePointer(client);
}

void testChangePointer() {
    Client *client = new Client();
    std::cout << "Address of old client: " << client << std::endl;
    int i = 0;
    while (i < 10) {
        changeFd(client, i);
        std::cout << "Address of new client: " << client << std::endl;
        i++;
    }
    delete client;
}
