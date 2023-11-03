#ifndef SENDER_H
#define SENDER_H

#include <utility>

#include "protocolo.h"
#include "queue.h"
#include "thread.h"
#include "defs.h"

class Sender: public Thread {
    // Queue<Mensaje>& queue;
    Protocolo& protocol;

    Queue<EstadoDelJuego> estadoAEnviar;

public:
    void run() override;
    //Sender(Queue<Mensaje>& msg_queue, Protocol& protocol_ref, std::atomic<bool>& is_alive);
    Sender(Protocolo& protocolo);

    // //Logica de lobby
    // void enviarMapas(std::vector<std::string> mapasDisponibles);
    // void enviarConfirmacion(id idPartida);
};

#endif
