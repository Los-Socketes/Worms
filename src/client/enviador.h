#ifndef ENVIADOR_H_
#define ENVIADOR_H_

#include <atomic>
#include <iostream>
#include <string>
#include <syslog.h>

#include "thread.h"
#include "queue.h"
#include "protocolo.h"
#include "defs.h"

class Enviador: public Thread {
private:
    Protocolo& protocolo;
    Queue<std::string>& envio_comandos;
    std::atomic<bool> cont;

public:
    Enviador(Protocolo& protocolo, Queue<std::string>& envio_comandos);
    // Enviador(Queue<std::string>& envio_comandos);

    // Hilo que se encarga de enviar los comandos al servidor.
    void run() override;

    // Para cerrar el hilo.
    void stop();

};

#endif  // ENVIADOR_H_
