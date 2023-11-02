#include "sender.h"


Sender::Sender(Protocolo& protocolo) 
    : protocol(protocolo) {
}


void Sender::run() {
}

// void Sender::enviarMapas(std::vector<std::string> mapasDisponibles) {
//     this->protocol.enviarMapas(mapasDisponibles);
// }

// void Sender::enviarConfirmacion(id idPartida) {
//     this->protocol.enviarConfirmacion(idPartida);
// }
