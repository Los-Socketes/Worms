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
        try {
            nuevoEstado = this->estadoAEnviar.pop();
            this->protocol.enviarEstadoDelJuego(nuevoEstado);
        }
        // atrapo error de que se cierre la queue
        catch(const ClosedQueue e)
        {
            // std::cerr << e.what() << '\n';
            return;
        }
        
    }
}


void Sender::stop() {
    this->estadoAEnviar.close();
}
