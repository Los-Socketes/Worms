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
        std::vector<id> opciones_partida;
        std::vector<RepresentacionMapa> opciones_mapa;
        switch (opcion){
            case 1:
                protocolo.pedirInformacion(MAPA);
                opciones_mapa = protocolo.obtenerMapas();
                std::cout << "Mapas disponibles:" << std::endl;
                for (const RepresentacionMapa& mapa : opciones_mapa) {
                    std::cout << std::to_string(pos) << ") " << mapa.nombre << std::endl;
                    pos++;
                }
                std::cin >> sub_opcion;
                protocolo.crearPartida(opciones_mapa[sub_opcion].ID);
                continuar_menu = false;
                break;
            case 2:
                protocolo.pedirInformacion(MAPA);
                opciones_partida = protocolo.obtenerPartidas();
                std::cout << "Partidas disponibles:" << std::endl;
                for (const id& id_partida : opciones_partida) {
                    std::cout << std::to_string(pos) << ") " << id_partida << std::endl;
                    pos++;
                }
                std::cin >> sub_opcion;
                protocolo.unirseAPartida(opciones_partida[sub_opcion]);
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

