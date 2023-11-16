#include "accioncliente.h"

AccionMoverGusano::AccionMoverGusano(Direccion direccion) : 
    direccion(direccion) {}

void AccionMoverGusano::enviar(Protocolo& protocolo) {
    protocolo.moverGusano(direccion);
}

AccionEquiparArma::AccionEquiparArma(ArmaProtocolo arma) : arma(arma) {}

void AccionEquiparArma::enviar(Protocolo& protocolo) {
    protocolo.equiparArma(arma);
}

AccionDisparar::AccionDisparar() {}

void AccionDisparar::enviar(Protocolo& protocolo) {
    protocolo.atacar();
}

