#include "entradateclado.h"

EntradaTeclado::EntradaTeclado(Queue<std::string>& envio_comandos, Queue<std::string>& comandos_teclado)
    : envio_comandos(envio_comandos), comandos_teclado(comandos_teclado), cont(true) {}

void EntradaTeclado::run() {
    try {
        // TODO: Implementar SDL para leer los comandos del teclado.
    } catch (const ClosedQueue& e) {
        syslog(LOG_INFO, "%s", e.what());
    }
}

void EntradaTeclado::stop() {
    cont = false;
}
