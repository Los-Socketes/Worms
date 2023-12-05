#include "mapa.h"
#include <iostream>
#include <fstream>



Mapa::Mapa(YAML::Node& config, std::string nombre) {
    int cantVigas = config[nombre]["Cantidad vigas"].as<int>();
    this->dimensiones = config[nombre]["Dimensiones"].as<dimensionesMapa>();
    for (int i = 1; i <= cantVigas; i++) {
        std::stringstream viga;
        viga << "Viga " << i;
        Viga vigaActual;
        vigaActual.tamanio = config[nombre][viga.str()]["Tamanio"].as<int>();
        vigaActual.coordenadas = config[nombre][viga.str()]["Coordenadas"].as<coordsVigas>();
        vigaActual.angulo = config[nombre][viga.str()]["Angulo"].as<float>();

        this->vigas.push_back(vigaActual);
    }

    this->posicionGusanos = config[nombre]["Posicion gusanos"].as<std::vector<coordsGusanos>>();
    this->cantGusanos = this->posicionGusanos.size();
}


Mapas::Mapas() {
    std::ifstream fin("./config.yaml");
    YAML::Node config = YAML::Load(fin);

    std::vector<std::string> nombresMapas = config["Nombres mapas"].as<std::vector<std::string>>();
    for (auto &&nombreMapa : nombresMapas) {
        Mapa mapaActual(std::ref(config), nombreMapa);

        this->mapas.push_back(mapaActual);
    }
    fin.close(); 
    this->nombresMapas = nombresMapas;
}

Mapa Mapas::at(int index) {
    return this->mapas.at(index);
}

