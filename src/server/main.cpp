#include <cstring>
#include <iostream>
#include "server.h"

#define BINARIO "./server"

int main(int argc, char* argv[]) {
    std::cout << "Hola mundo desde el server\n";

    if (strcmp(argv[0], BINARIO) != 0) {
        std::cout << argv[0];
        std::cerr << "Algo salio terriblemente mal\n";
    }

    if (argc != 2) {
        std::cout << "Usar como: " << BINARIO << " <ip/hostname server> "
	        << std::endl;
        return -1;
    }

    Server server(argv[1]);
    server.esperarQueSeCierre();

}
