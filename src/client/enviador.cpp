#include "enviador.h"

Enviador::Enviador(Protocolo& protocolo, Queue<std::string>& envio_comandos) :
    protocolo(protocolo), envio_comandos(envio_comandos) , cont(true) {}

// Enviador::Enviador(Queue<std::string>& envio_comandos) :
//     envio_comandos(envio_comandos) , cont(true) {}

void Enviador::run() {
    try {
        cont = true;
        while(cont){
            // TODO: Enviar comandos al servidor por protocolo.
            std::string comando = envio_comandos.pop();
            if (comando == "Presionando izquierda") {
                protocolo.moverGusano(0, INICIO_IZQ);
            } else if (comando == "Presionando derecha") {
                protocolo.moverGusano(0, INICIO_DER);
            } else if (comando == "Soltando izquierda") {
                protocolo.moverGusano(0, FIN_IZQ);
            } else if (comando == "Soltando derecha") {
                protocolo.moverGusano(0, FIN_DER);
            } else {
                // Por ahora solo tomo comandos de movimiento.
                // El resto de los comandos se imprimen.
                std::cout << comando << std::endl;
            }
        }
    } catch (const ClosedQueue& e) {
        syslog(LOG_INFO, "%s", e.what());
    }
     
}

void Enviador::stop() {
    cont = false;
}
