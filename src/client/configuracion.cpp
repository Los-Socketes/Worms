#include "configuracion.h"

Configuracion::Configuracion() {
    std::ifstream fin("../src/client/configCliente.yaml");
    YAML::Node config = YAML::Load(fin);

    this->dimensionesIniciales.enX = config["Dimensiones pantalla"]["ancho"].as<int>();
    this->dimensionesIniciales.enY = config["Dimensiones pantalla"]["alto"].as<int>();
    this->volumenInicial = config["Volumen inicial"].as<int>();
    fin.close();
}


std::pair<int, int> Configuracion::getDimensionesIniciales() {
    return this->dimensionesIniciales;
}


int Configuracion::getVolumenIncial() {
    this->volumenInicial;
}
