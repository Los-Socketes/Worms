#include "accionCliente.h"

AccionMoverGusano::AccionMoverGusano(Direccion direccion) : 
    direccion(direccion) {}

void AccionMoverGusano::enviar(Protocolo& protocolo, std::atomic<bool>& es_turno) {
    if (es_turno)
        protocolo.moverGusano(direccion);
}

AccionCambiarAngulo::AccionCambiarAngulo(radianes angulo) : 
    angulo(angulo) {}

void AccionCambiarAngulo::enviar(Protocolo& protocolo, std::atomic<bool>& es_turno) {
    if (es_turno)
        protocolo.configurarAngulo(angulo);
}

AccionCambiarCoordenadas::AccionCambiarCoordenadas(coordX x, coordY y) :
    x(x),
    y(y) {}

void AccionCambiarCoordenadas::enviar(Protocolo& protocolo, std::atomic<bool>& es_turno) {
    if (es_turno)
        protocolo.configurarCoordenadas(std::make_pair(x, y));
}

AccionCambiarPotencia::AccionCambiarPotencia(float potencia) :
    potencia(potencia) {}

void AccionCambiarPotencia::enviar(Protocolo& protocolo, std::atomic<bool>& es_turno) {
    if (es_turno)
        protocolo.configurarPotencia(potencia);
}

AccionCambiarCuentaRegresiva::AccionCambiarCuentaRegresiva(int cuenta_regresiva) :
    cuenta_regresiva(cuenta_regresiva) {}

void AccionCambiarCuentaRegresiva::enviar(Protocolo& protocolo, std::atomic<bool>& es_turno) {
    if (es_turno)
        protocolo.configurarCuentaRegresiva(cuenta_regresiva);
}

AccionEquiparArma::AccionEquiparArma(ArmaProtocolo arma) :
    arma(arma) {}

void AccionEquiparArma::enviar(Protocolo& protocolo, std::atomic<bool>& es_turno) {
    if (es_turno)
        protocolo.equiparArma(arma);
}

AccionDisparar::AccionDisparar() {}

void AccionDisparar::enviar(Protocolo& protocolo, std::atomic<bool>& es_turno) {
    if (es_turno)
        protocolo.atacar();
}

void AccionEmpezarPartida::enviar(Protocolo& protocolo, std::atomic<bool>& es_turno) {
    protocolo.iniciarPartida();
}

AccionCheat::AccionCheat(TipoCheat cheat) :
    cheat(cheat) {}

void AccionCheat::enviar(Protocolo& protocolo, std::atomic<bool>& es_turno) {
    if (cheat == TipoCheat::ARRANCAR_C)
        protocolo.enviarCheat(cheat);
    else if (es_turno)
        protocolo.enviarCheat(cheat);
}

