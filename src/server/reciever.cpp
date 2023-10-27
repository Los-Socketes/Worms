#include "reciever.h"


Reciever::Reciever(Protocolo& protocol, Queue<Direccion>& acciones)
    : protocolo(protocol),
      acciones(acciones) {
}

void Reciever::run() {
    //TODO Cambiar a socket vivo o algo
    while (true) {
        Direccion accionDeseada;
        accionDeseada = this->protocolo.recibirAccion();

        this->acciones.push(accionDeseada);
    }
}
