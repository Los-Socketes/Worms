#include <cstring>
#include <iostream>
#include "server.h"
#include "configServer.h"

#define BINARIO "./server"

#define ERROR_M 1
#define SUCCESS_M 0

ConfiguracionServer config;

int main(int argc, char* argv[]) {
    srand(time(NULL));
    try {
        if (strcmp(argv[0], BINARIO) != 0) {
            std::cerr << "Algo salio terriblemente mal\n";
        }

        if (argc != 2) {
            std::cout << "Usar como: " << BINARIO << " <ip/hostname server> "
                << std::endl;
        }

        ConfiguracionServer config = ConfiguracionServer();

        Server server(argv[1]);
        server.esperarQueSeCierre();
        return SUCCESS_M;
    } catch (const std::exception& err) {
        std::cerr << "Algo salio mal y se atrapo una excepcion: " << err.what() << "\n";
        return ERROR_M;
    } catch (...) {
        std::cerr << "Algo salio mal y se atrapo una excepcion desconocida.\n";
        return ERROR_M;
    }
    

}
