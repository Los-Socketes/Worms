#include "sender.h"


Sender::Sender(Protocolo& protocolo) 
    : protocol(protocolo) {
}

void Sender::anadirEstado(EstadoDelJuego estado) {
    this->estadoAEnviar.push(estado);
}

void Sender::enviarEstado() {
    //TODO emprolijar, sacar el true
    EstadoDelJuego nuevoEstado;
    while (true) {
        nuevoEstado = this->estadoAEnviar.pop();
        this->protocol.enviarEstadoDelJuego(nuevoEstado);
    }
}
