#include "menu.h"

Menu::Menu(Protocolo& protocolo) : protocolo(protocolo) {}

bool Menu::ejecutar() {
    bool continuar = true;
    bool continuar_menu = true;
    while(continuar_menu) {
        std::cout << "Eliga una opción:" << std::endl;
        std::cout << "1) Crear partida." << std::endl;
        std::cout << "2) Unirse a partida." << std::endl;
        std::cout << "3) Salir." << std::endl;
        int opcion;
        std::cin >> opcion;
        int pos = 0;
        int sub_opcion;
        std::vector<id> opciones;
        switch (opcion){
            case 1:
                protocolo.pedirInformacion(MAPA);
                opciones = protocolo.obtenerMapas();
                std::cout << "Mapas disponibles:" << std::endl;
                for (const id& id_mapa : opciones) {
                    std::cout << std::to_string(pos) << ") " << id_mapa << std::endl;
                    pos++;
                }
                std::cin >> sub_opcion;
                protocolo.crearPartida(opciones[sub_opcion]);
                continuar_menu = false;
                break;
            case 2:
                protocolo.pedirInformacion(MAPA);
                opciones = protocolo.obtenerPartidas();
                std::cout << "Partidas disponibles:" << std::endl;
                for (const id& id_partida : opciones) {
                    std::cout << std::to_string(pos) << ") " << id_partida << std::endl;
                    pos++;
                }
                std::cin >> sub_opcion;
                protocolo.unirseAPartida(opciones[sub_opcion]);
                continuar_menu = false;
                break;
            default:
                continuar_menu = false;
                continuar = false;
                break;
        }
    }
    return continuar;
}

