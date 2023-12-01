#ifndef MAPA_HEADER
#define MAPA_HEADER


#include "defs.h"
#include "yaml-cpp/yaml.h"

typedef std::pair<coordX, coordY> coordsGusanos;
typedef std::pair<coordX, coordY> coordsVigas;

struct Viga {
    int tamanio;
    std::pair<coordX, coordY> coordenadas;
    float angulo;

};

struct Mapa {
    std::vector<Viga> vigas;
    std::vector<coordsGusanos> posicionGusanos;

    Mapa(YAML::Node& config, std::string nombre);
};

struct Mapas {
    std::vector<Mapa> mapas;

    Mapas();
};

#endif
