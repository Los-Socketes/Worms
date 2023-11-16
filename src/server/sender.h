#ifndef SENDER_H
#define SENDER_H

#include <utility>

#include "protocolo.h"
#include "queue.h"
#include "thread.h"
#include "defs.h"

#define enviarEstado run

class Sender: public Thread {
    // Queue<Mensaje>& queue;
    Protocolo& protocol;

    Queue<std::shared_ptr<EstadoDelJuego>> estadoAEnviar;

    void enviarEstado() override;
public:
    //Sender(Queue<Mensaje>& msg_queue, Protocol& protocol_ref, std::atomic<bool>& is_alive);
    Sender(Protocolo& protocolo);

    void anadirEstado(std::shared_ptr<EstadoDelJuego> estado);

    void stop() override;
    // //Logica de lobby
    // void enviarMapas(std::vector<std::string> mapasDisponibles);
    // void enviarConfirmacion(id idPartida);
};

#endif
