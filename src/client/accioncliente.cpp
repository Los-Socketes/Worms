#include "accioncliente.h"

AccionMoverGusano::AccionMoverGusano(int id_gusano, Direccion direccion) : 
    id_gusano(id_gusano), direccion(direccion) {}

void AccionMoverGusano::enviar(Protocolo& protocolo) {
    protocolo.moverGusano(id_gusano, direccion);
}

AccionDisparar::AccionDisparar(int id_gusano) : id_gusano(id_gusano) {}

void AccionDisparar::enviar(Protocolo& protocolo) {
    //protocolo.disparar(id_gusano);
}

