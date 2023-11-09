#ifndef ENTRADATECLADO_H_
#define ENTRADATECLADO_H_

#include <atomic>
#include <iostream>
#include <string>
#include <syslog.h>
#include <SDL2pp/SDL2pp.hh>

#include "accioncliente.h"
#include "thread.h"
#include "queue.h"

class EntradaTeclado: public Thread {
private:
    Queue<std::shared_ptr<AccionCliente>>& envio_comandos;
    Queue<Comando>& comandos_teclado;
    std::atomic<bool> cont;

public:
    EntradaTeclado(Queue<std::shared_ptr<AccionCliente>>& envio_comandos, Queue<Comando>& comandos_teclado);

    // Hilo que se encarga de leer los comandos del teclado y enviarlos al servidor o
    // a la cola de comandos locales de teclado.
    void run() override;

    // Para cerrar el hilo.
    void stop() override;

};

#endif  // ENTRADATECLADO_H_
