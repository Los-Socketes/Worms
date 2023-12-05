#ifndef SENDER_H
#define SENDER_H

#include <utility>

#include "protocolo.h"
#include "queue.h"
#include "thread.h"
#include "defs.h"

#define enviarEstado run

class Sender: public Thread {
    Protocolo& protocol;

    Queue<std::shared_ptr<EstadoDelJuego>> estadoAEnviar;

    void enviarEstado() override;
public:
    Sender(Protocolo& protocolo);

    void anadirEstado(std::shared_ptr<EstadoDelJuego> estado);

    void stop() override;
};

#endif
