#include "arma.h"

#include "yaml-cpp/yaml.h"


std::pair<int,int> leerArchivo(const std::string& arma, CaracteristicasArma& caracteristicas, Danio& danio, Danio& danioFragmento) {
    caracteristicas.tieneMira = config[arma]["ConMira"].as<bool>();
    caracteristicas.esCuerpoACuerpo = config[arma]["EsCuerpoACuerpo"].as<bool>();
    caracteristicas.tienePotenciaVariable = config[arma]["TienePotenciaVariable"].as<bool>();
    caracteristicas.tieneCuentaRegresiva = config[arma]["TieneCuentaRegresiva"].as<bool>();
    caracteristicas.esTeledirigido = config[arma]["EsTeledirigido"].as<bool>();
    caracteristicas.esAfectadoPorViento = config[arma]["EsAfectadaPorViento"].as<bool>();
    
    int municiones = config[arma]["Municiones"].as<int>();
    int fragmentos = config[arma]["CantidadFragmentos"].as<int>();

    danio.epicentro = config[arma]["Danio"]["Epicentro"].as<int>();
    danio.radio = config[arma]["Danio"]["Radio"].as<int>();

    if (fragmentos > 0) {
        danioFragmento.epicentro = config[arma]["DanioFragmento"]["Epicentro"].as<int>();
        danioFragmento.radio = config[arma]["DanioFragmento"]["Radio"].as<int>();
    }

    std::pair<int,int> municionesYFragmentos(municiones, fragmentos);
    return municionesYFragmentos;
}


Arma::Arma(ArmaProtocolo idArma) {
    YAML::Node config = YAML::LoadFile("config.yaml");
    CaracteristicasArma caracteristicas;
    Danio danio;
    Danio danioFragmento;
    std::pair<int,int> municionesYFragmentos(-1,0);
    switch (idArma) {
    case BAZOOKA_P:{
        municionesYFragmentos = leerArchivo("Bazooka", caracteristicas, danio, danioFragmento);
        break;
        }
    case MORTERO_P:{
        municionesYFragmentos = leerArchivo("Mortero", caracteristicas, danio, danioFragmento);
        break;
        }
    case GRANADA_VERDE_P:{
        municionesYFragmentos = leerArchivo("Granada Verde", caracteristicas, danio, danioFragmento);
        break;
        }
    case GRANADA_ROJA_P:{
        municionesYFragmentos = leerArchivo("Granada Roja", caracteristicas, danio, danioFragmento);
        break;
        }
    case GRANADA_SANTA_P:{
        municionesYFragmentos = leerArchivo("Granada Santa", caracteristicas, danio, danioFragmento);
        break;
        }
    case BANANA_P:{
        municionesYFragmentos = leerArchivo("Banana", caracteristicas, danio, danioFragmento);
        break;
        }
    case DINAMITA_P:{
        municionesYFragmentos = leerArchivo("Dinamita", caracteristicas, danio, danioFragmento);
        break;
        }
    case BATE_P:{
        municionesYFragmentos = leerArchivo("Bate de Baseball", caracteristicas, danio, danioFragmento);
        break;
        }
    case ATAQUE_AEREO_P:{
        municionesYFragmentos = leerArchivo("Ataque Aereo", caracteristicas, danio, danioFragmento);
        break;
        }
    case TELETRANSPORTACION_P:{
        municionesYFragmentos = leerArchivo("Teletransportacion", caracteristicas, danio, danioFragmento);
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
    this->idArma = idArma;
    this->caracteristicas = caracteristicas;
    this->municiones = municionesYFragmentos.first;
    this->fragmentos = municionesYFragmentos.second;
    this->danio = danio;
    this->danioFragmento = danioFragmento;
    this->potencia = 1;
    this->cuentaRegresiva = 0;
    this->angulo = 0;
}


Danio Arma::getDanio() {
    return this->danio;
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