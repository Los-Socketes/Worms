#include "accioncliente.h"

AccionMoverGusano::AccionMoverGusano(int id_gusano, Direccion direccion) : 
    id_gusano(id_gusano), direccion(direccion) {}

void AccionMoverGusano::enviar(Protocolo& protocolo) {
    protocolo.moverGusano(id_gusano, direccion);
}

AccionEquiparArma::AccionEquiparArma(int id_gusano, ArmaProtocolo arma) : 
    id_gusano(id_gusano), arma(arma) {}

void AccionEquiparArma::enviar(Protocolo& protocolo) {
    protocolo.equiparArma(id_gusano, arma);
}

AccionDisparar::AccionDisparar(int id_gusano) : id_gusano(id_gusano) {}

void AccionDisparar::enviar(Protocolo& protocolo) {
    protocolo.atacar(id_gusano);
}

