#include "arma.h"

Arma::Arma(ArmaProtocolo idArma) {
    // TODO: hacer constructores de todas las clases
    this->idArma = idArma;
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