#include "enviador.h"

// Enviador::Enviador(Protocolo& protocolo, Queue<std::string>& envio_comandos) :
    // protocolo(protocolo), envio_comandos(envio_comandos) , cont(true) {}

Enviador::Enviador(Queue<std::string>& envio_comandos) :
    envio_comandos(envio_comandos) , cont(true) {}

void Enviador::run() {
    try {
        while(cont){
            // TODO: Enviar comandos al servidor por protocolo.
            // Por ahora solo los imprimo.
            std::string comando = envio_comandos.pop();
            std::cout << "Enviando comando: " << comando << std::endl;                        
        }
    } catch (const ClosedQueue& e) {
        syslog(LOG_INFO, "%s", e.what());
    }
     
}

void Enviador::stop() {
    cont = false;
}
