#ifndef MAPA_HEADER
#define MAPA_HEADER

#include "defs.h"
#include "yaml-cpp/yaml.h"

typedef std::pair<coordX, coordY> coordsGusanos;
typedef std::pair<coordX, coordY> coordsVigas;
typedef std::pair<coordX, coordY> dimensionesMapa;

struct Viga {
    int tamanio;
    coordsVigas coordenadas;
    float angulo;

};

struct Mapa {
    std::vector<Viga> vigas;
    std::vector<coordsGusanos> posicionGusanos;
    int cantGusanos;
    dimensionesMapa dimensiones;

    Mapa(YAML::Node& config, std::string nombre);
    Mapa(){}
};

struct Mapas {
    std::vector<Mapa> mapas;
    std::vector<std::string> nombresMapas;

    Mapas();
    Mapa at(int index);
};

#endif
