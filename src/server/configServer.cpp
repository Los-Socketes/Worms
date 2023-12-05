#include "configServer.h"
#include <fstream>

#include "yaml-cpp/yaml.h"

ConfiguracionServer::ConfiguracionServer() {
    std::ifstream fin("../src/common/config.yaml");
    YAML::Node config = YAML::Load(fin);

    this->dimensionesIniciales.enX = config["Dimensiones pantalla"]["ancho"].as<int>();
    this->dimensionesIniciales.enY = config["Dimensiones pantalla"]["alto"].as<int>();
    this->volumenInicial = config["Volumen inicial"].as<int>();

    int cantJugadores = config["Cantidad maxima jugadores"].as<int>();
    for (int i = 1; i <= cantJugadores; i++) {
        std::stringstream jugador;
        jugador << "Jugador " << i;
        colorJugador colores = config["Color jugadores"][jugador.str()].as<colorJugador>();
        this->coloresJugadores.push_back(colores);
    }
    this->pixelsPorMetro = config["Pixels por metro"].as<int>();
    fin.close();
}