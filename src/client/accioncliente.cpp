#include "accioncliente.h"

AccionMoverGusano::AccionMoverGusano(Direccion direccion) : 
    direccion(direccion) {}

void AccionMoverGusano::enviar(Protocolo& protocolo) {
    protocolo.moverGusano(direccion);
}

AccionCambiarAngulo::AccionCambiarAngulo(radianes angulo) : 
    angulo(angulo) {}

void AccionCambiarAngulo::enviar(Protocolo& protocolo) {
    protocolo.configurarAngulo(angulo);
}

AccionCambiarCoordenadas::AccionCambiarCoordenadas(coordX x, coordY y) :
    x(x),
    y(y) {}

void AccionCambiarCoordenadas::enviar(Protocolo& protocolo) {
    protocolo.configurarCoordenadas(std::make_pair(x, y));
}

AccionCambiarPotencia::AccionCambiarPotencia(float potencia) :
    potencia(potencia) {}

void AccionCambiarPotencia::enviar(Protocolo& protocolo) {
    protocolo.configurarPotencia(potencia);
}

AccionCambiarCuentaRegresiva::AccionCambiarCuentaRegresiva(int cuenta_regresiva) :
    cuenta_regresiva(cuenta_regresiva) {}

void AccionCambiarCuentaRegresiva::enviar(Protocolo& protocolo) {
    protocolo.configurarCuentaRegresiva(cuenta_regresiva);
}

AccionEquiparArma::AccionEquiparArma(ArmaProtocolo arma) :
    arma(arma) {}

void AccionEquiparArma::enviar(Protocolo& protocolo) {
    protocolo.equiparArma(arma);
}

AccionDisparar::AccionDisparar() {}

void AccionDisparar::enviar(Protocolo& protocolo) {
    protocolo.atacar();
}

