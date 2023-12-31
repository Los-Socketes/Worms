#include "sender.h"
#include "liberror.h"


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
            if (!this->protocol.enviarEstadoDelJuego(nuevoEstado)) {
                break;
            }
        }
        // atrapo error de que se cierre la queue
        catch(const ClosedQueue& e)
        {
            return;
        }
        catch(const LibError& e)
        {
            return;
        }
        
    }
}


void Sender::stop() {
    if (!this->estadoAEnviar.is_closed()) {
        this->estadoAEnviar.close();
    }
}
