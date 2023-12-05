#include "configuracionCliente.h"
#include <fstream>

#include "yaml-cpp/yaml.h"

ConfiguracionCliente::ConfiguracionCliente() {
    std::ifstream fin("assets/config/config.yaml");
    YAML::Node config = YAML::Load(fin);

    this->viento_max = config["Viento rango"]["Max"].as<int>();
    this->viento_min = config["Viento rango"]["Min"].as<int>();
    this->tiempo_turno = config["Tiempo turno"].as<int>();
    this->tiempo_changui = config["Tiempo changui"].as<int>();

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

int ConfiguracionCliente::getVientoMax() {
    return this->viento_max;
}

int ConfiguracionCliente::getVientoMin() {
    return this->viento_min;
}

int ConfiguracionCliente::getTiempoTurno() {
    return this->tiempo_turno;
}

int ConfiguracionCliente::getTiempoChangui() {
    return this->tiempo_changui;
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
