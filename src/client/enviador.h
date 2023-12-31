#ifndef ENVIADOR_H_
#define ENVIADOR_H_

#include <atomic>
#include <iostream>
#include <string>
#include <syslog.h>

#include "accionCliente.h"
#include "thread.h"
#include "queue.h"
#include "protocolo.h"
#include "defs.h"

// Hilo que se encarga de enviar los comandos al servidor.
class Enviador: public Thread {
private:
    Protocolo& protocolo;
    Queue<std::shared_ptr<AccionCliente>>& envio_comandos;
    std::atomic<bool> cont;
    std::atomic<bool>& es_turno;

public:
    Enviador(Protocolo& protocolo,
        Queue<std::shared_ptr<AccionCliente>>& envio_comandos,
        std::atomic<bool>& es_turno);
    
    void run() override;

    // Para cerrar el hilo.
    void stop() override;

};

#endif  // ENVIADOR_H_
