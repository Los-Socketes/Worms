#include "reciever.h"
#include "protocolo.h"
#include <cstdlib>


Reciever::Reciever(Protocolo& protocol) 
    : protocolo(protocol)
      // acciones(acciones)
{
}

void Reciever::darAcceso(Queue<Direccion> *accionesRecibidas) {
    //TODO Throw
    if (accionesRecibidas == nullptr)
        abort();

    this->acciones = accionesRecibidas;
}

void Reciever::run() {
    //TODO Cambiar a socket vivo o algo
    while (true) {
        Direccion accionDeseada;
        accionDeseada = this->protocolo.obtenerAccion();

        this->acciones->push(accionDeseada);
    }
}
