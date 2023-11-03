#include "sender.h"


Sender::Sender(Protocolo& protocolo) 
    : protocol(protocolo) {
}


void Sender::run() {
    //TODO emprolijar, sacar el true
    while (true) {
        EstadoDelJuego ultimo;
        ultimo = this->estadoAEnviar.pop();
        this->protocol.enviarEstadoDelJuego(ultimo);
    }
}
