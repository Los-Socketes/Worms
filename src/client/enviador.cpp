#include "enviador.h"

Enviador::Enviador(Queue<std::string>& envio_comandos) :
    envio_comandos(envio_comandos) , cont(true) {}

void Enviador::run() {
    try {
        // TODO: Enviar comandos al servidor por protocolo.
        // Por ahora no hago nada con ellos.
        while(cont){
            envio_comandos.pop();
        }
    } catch (const ClosedQueue& e) {
        syslog(LOG_INFO, "%s", e.what());
    }
     
}

void Enviador::stop() {
    cont = false;
}
