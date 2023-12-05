#include "configServer.h"
#include <fstream>

#include "yaml-cpp/yaml.h"

ConfiguracionServer::ConfiguracionServer() {
    std::ifstream fin("assets/config/config.yaml");
    YAML::Node config = YAML::Load(fin);

    this->gravedad.enX = config["Gravedad"]["X"].as<float>();
    this->gravedad.enY = config["Gravedad"]["Y"].as<float>();

    this->viento.enX = config["Viento"]["X"].as<int>();
    this->viento.enY = config["Viento"]["Y"].as<int>();

    this->metrosSinDanio = config["Metros sin danio"].as<float>();

    this->velocidadMovimiento = config["Velocidad movimiento"].as<float>();

    this->potenciaSalto = config["Potencia salto"].as<float>();

    this->vidaGusano = config["Vida gusano"].as<int>();

    this->esperaDinamita = config["Tiempo espera dinamita"].as<int>();

    this->esperaProvision = config["Tiempo espera provision"].as<int>();

    this->esperaViento = config["Tiempo espera viento"].as<int>();

    fin.close();
}
