#include <iostream>
#include <string>
#include <syslog.h>

#include "socket.h"
#include "cliente.h"
#include "configuracionCliente.h"

#define HOST 1
#define PUERTO 2

int main(int argc, char* argv[]) {
    if (strcmp(argv[0], "./client") != 0) {
        std::cout << argv[0];
        std::cerr << "Algo salio terriblemente mal\n";
    }

    if (argc != 3) {
        std::cout << "Usar como: client <ip/hostname server> ";
        std::cout << "<puerto/servicename>" << std::endl;
        return 1;
    }
    // try {
        // Inicializo la configuracion del cliente.
        ConfiguracionCliente config;

        // Inicializo el socket.
        Socket socket(argv[HOST], argv[PUERTO]);

        // Inicializo el cliente.
        Cliente cliente(std::move(socket), config);
        InformacionInicial informacion = cliente.ejecutar_menu(argc, argv);
        if (!cliente.salioDesdeMenu() && informacion.jugador != -1) {
            cliente.loop_principal(informacion);
        }      
    // } catch (const std::exception& e) {
    //     syslog(LOG_CRIT, "[Crit] Error: %s\n", e.what());
    // } catch (...) {
    //     syslog(LOG_CRIT, "[Crit] Unknown error.\n");
    // }
    return 0;
}
