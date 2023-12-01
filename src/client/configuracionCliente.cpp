#include "configuracionCliente.h"
#include <fstream>

#include "yaml-cpp/yaml.h"

ConfiguracionCliente::ConfiguracionCliente() {
    std::ifstream fin("./configCliente.yaml");
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
    //this->pixelsPorMetro = config["Pixeles por metro"].as<int>();
    fin.close();
}


std::pair<int, int> ConfiguracionCliente::getDimensionesIniciales() {
    return this->dimensionesIniciales;
}


int ConfiguracionCliente::getVolumenInicial() {
    return this->volumenInicial;
}


std::vector<colorJugador>& ConfiguracionCliente::getColoresJugadores() {
    return this->coloresJugadores;
}


colorJugador ConfiguracionCliente::getColorJugador(int numJugador) {
    return this->coloresJugadores[numJugador - 1];
}


int ConfiguracionCliente::getPixelsPorMetro() {
    return this->pixelsPorMetro;
}
