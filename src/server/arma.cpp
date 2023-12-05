#include "arma.h"
#include <iostream>
#include <fstream>

#include "yaml-cpp/yaml.h"


std::tuple<int,int,int> leerArchivo(YAML::Node& config, const std::string& arma, CaracteristicasArma& caracteristicas, Danio& danio, Danio& danioFragmento) {
    caracteristicas.tieneMira = config[arma]["ConMira"].as<bool>();
    caracteristicas.esCuerpoACuerpo = config[arma]["EsCuerpoACuerpo"].as<bool>();
    caracteristicas.tienePotenciaVariable = config[arma]["TienePotenciaVariable"].as<bool>();
    caracteristicas.tieneCuentaRegresiva = config[arma]["TieneCuentaRegresiva"].as<bool>();
    caracteristicas.esTeledirigido = config[arma]["EsTeledirigido"].as<bool>();
    caracteristicas.esAfectadoPorViento = config[arma]["EsAfectadaPorViento"].as<bool>();
    
    int municiones = config[arma]["Municiones"].as<int>();
    int fragmentos = config[arma]["CantidadFragmentos"].as<int>();
    int proyectiles = config[arma]["CantidadProyectiles"].as<int>();

    danio.epicentro = config[arma]["Danio"]["Epicentro"].as<int>();
    danio.radio = config[arma]["Danio"]["Radio"].as<int>();

    if (fragmentos > 0) {
        danioFragmento.epicentro = config[arma]["DanioFragmento"]["Epicentro"].as<int>();
        danioFragmento.radio = config[arma]["DanioFragmento"]["Radio"].as<int>();
    }

    std::tuple<int,int,int> municionesFragmentosProyectiles(municiones, fragmentos, proyectiles);
    return municionesFragmentosProyectiles;
}


Arma::Arma(ArmaProtocolo idArma) {
    std::ifstream fin("assets/config/config.yaml");
    YAML::Node config = YAML::Load(fin);
    CaracteristicasArma caracteristicas;
    Danio danio;
    Danio danioFragmento;
    std::tuple<int,int,int> municionesFragmentosProyectiles(-1,0,1);
    switch (idArma) {
    case BAZOOKA_P:{
        municionesFragmentosProyectiles = leerArchivo(config, "Bazooka", caracteristicas, danio, danioFragmento);
        break;
        }
    case MORTERO_P:{
        municionesFragmentosProyectiles = leerArchivo(config, "Mortero", caracteristicas, danio, danioFragmento);
        break;
        }
    case GRANADA_VERDE_P:{
        municionesFragmentosProyectiles = leerArchivo(config, "Granada Verde", caracteristicas, danio, danioFragmento);
        break;
        }
    case GRANADA_ROJA_P:{
        municionesFragmentosProyectiles = leerArchivo(config, "Granada Roja", caracteristicas, danio, danioFragmento);
        break;
        }
    case GRANADA_SANTA_P:{
        municionesFragmentosProyectiles = leerArchivo(config, "Granada Santa", caracteristicas, danio, danioFragmento);
        break;
        }
    case BANANA_P:{
        municionesFragmentosProyectiles = leerArchivo(config, "Banana", caracteristicas, danio, danioFragmento);
        break;
        }
    case DINAMITA_P:{
        municionesFragmentosProyectiles = leerArchivo(config, "Dinamita", caracteristicas, danio, danioFragmento);
        break;
        }
    case BATE_P:{
        municionesFragmentosProyectiles = leerArchivo(config, "Bate de Baseball", caracteristicas, danio, danioFragmento);
        break;
        }
    case ATAQUE_AEREO_P:{
        municionesFragmentosProyectiles = leerArchivo(config, "Ataque Aereo", caracteristicas, danio, danioFragmento);
        break;
        }
    case TELETRANSPORTACION_P:{
        municionesFragmentosProyectiles = leerArchivo(config, "Teletransportacion", caracteristicas, danio, danioFragmento);
        break;
        }
    case NADA_P:{
        caracteristicas.tieneMira = false;
        caracteristicas.esCuerpoACuerpo = false;
        caracteristicas.tienePotenciaVariable = false;
        caracteristicas.tieneCuentaRegresiva = false;
        caracteristicas.esTeledirigido = false;
        caracteristicas.esAfectadoPorViento = false;
        danio.epicentro = 0;
        danio.radio = 0;
        break;
        }
    default:
        break;
    }
    fin.close();
    this->idArma = idArma;
    this->caracteristicas = caracteristicas;
    this->municiones = std::get<0>(municionesFragmentosProyectiles);
    this->fragmentos = std::get<1>(municionesFragmentosProyectiles);
    this->proyectiles = std::get<2>(municionesFragmentosProyectiles);
    this->danio = danio;
    this->danioFragmento = danioFragmento;
    this->potencia = caracteristicas.tienePotenciaVariable ? 0 : 100;
    this->cuentaRegresiva = caracteristicas.tieneCuentaRegresiva ? 5 : 0;
    this->angulo = 0;
    
}


Danio Arma::getDanio() {
    return this->danio;
}

void Arma::anadirMuniciones(int cantIdad) {
    this->municiones += cantIdad;
}

int Arma::getMuniciones() {
    return this->municiones;
}

int Arma::getFragmentos() {
    return this->fragmentos;
}

CaracteristicasArma Arma::getCaracteristicas() {
    return this->caracteristicas;
}

// Estos getters y setters no los usan todos, pero son genericos
// a muchas armas
Danio Arma::getDanioFragmento() {
    return this->danioFragmento;
}


float Arma::getPotencia() {
    return this->potencia;
}

void Arma::setPotencia(float potencia) {
    this->potencia = potencia;
}


int Arma::getCuentaRegresiva() {
    return this->cuentaRegresiva;
}

void Arma::setCuentaRegresiva(int cuentaRegresiva) {
    this->cuentaRegresiva = cuentaRegresiva;
}


float Arma::getAngulo() {
    return this->angulo;
}

void Arma::setAngulo(float angulo) {
    this->angulo = angulo;
}


RepresentacionArma Arma::getRepresentacion() {
    RepresentacionArma arma;
    arma.tieneMira = this->caracteristicas.tieneMira;
    arma.tienePotenciaVariable = this->caracteristicas.tienePotenciaVariable;
    arma.tieneCuentaRegresiva = this->caracteristicas.tieneCuentaRegresiva;
    arma.municiones = this->municiones;
    arma.fragmentos = this->fragmentos;
    arma.danio.epicentro = this->danio.epicentro;
    arma.danio.radio = this->danio.radio;
    arma.danioFragmento.epicentro = this->danioFragmento.epicentro;
    arma.danioFragmento.radio = this->danioFragmento.radio;
    arma.anguloRad = this->angulo;
    arma.potencia = this->potencia;
    arma.cuentaRegresiva = this->cuentaRegresiva;
    arma.arma = this->idArma;
    return arma;
}

std::pair<coordX, coordY> Arma::getCoordenadasTeletransporte() {
    return this->coordenadasTeletransporte;
}
void Arma::setCoordenadasTeletransporte(std::pair<coordX, coordY> coordenadas) {
    this->coordenadasTeletransporte = coordenadas;
}

void Arma::usar() {
    this->potencia = 0;
    this->cuentaRegresiva = 5;
    // this->angulo = 0;
    if (municiones <= 0) {
        return;
    }
    this->municiones -= 1;

}

int Arma::getCantidadProyectiles() {
    return this->proyectiles;
}
