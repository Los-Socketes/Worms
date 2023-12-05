#include "enviador.h"

Enviador::Enviador(Protocolo& protocolo,
    Queue<std::shared_ptr<AccionCliente>>& envio_comandos,
    std::atomic<bool>& es_turno) :
    protocolo(protocolo),
    envio_comandos(envio_comandos),
    cont(true),
    es_turno(es_turno) {}
    
void Enviador::run() {
    try {
        cont = true;
        while(cont){
            std::shared_ptr<AccionCliente> accion = envio_comandos.pop();
            accion->enviar(protocolo, es_turno);
        }
    } catch (const ClosedQueue& e) {
        syslog(LOG_INFO, "%s", e.what());
    } 
}

void Enviador::stop() {
    cont = false;
}
