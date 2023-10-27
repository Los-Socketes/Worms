#ifndef ENVIADOR_H_
#define ENVIADOR_H_

#include <atomic>
#include <iostream>
#include <string>
#include <syslog.h>

#include "../common/thread.h"
#include "../common/queue.h"

class Enviador: public Thread {
private:
    Queue<std::string>& envio_comandos;
    std::atomic<bool> cont;

public:
    Enviador(Queue<std::string>& envio_comandos);

    // Hilo que se encarga de enviar los comandos al servidor.
    void run() override;

    // Para cerrar el hilo.
    void stop();

};

#endif  // ENVIADOR_H_
