#include "sender.h"


Sender::Sender(Protocolo& protocolo) 
    : protocol(protocolo) {
}

void Sender::anadirEstado(std::shared_ptr<EstadoDelJuego> estado) {
    this->estadoAEnviar.push(estado);
}

void Sender::enviarEstado() {
    //TODO emprolijar, sacar el true
    std::shared_ptr<EstadoDelJuego> nuevoEstado;
    while (!this->estadoAEnviar.is_closed()) {
        nuevoEstado = this->estadoAEnviar.pop();
        this->protocol.enviarEstadoDelJuego(nuevoEstado);
    }
}


void Sender::stop() {
    this->estadoAEnviar.close();
}
