#ifndef RECIEVER_H
#define RECIEVER_H

#include <utility>

#include "protocol.h"
#include "queue.h"
#include "thread.h"

class Reciever: public Thread {
    Queue<Mensaje>& queue;
    Protocol& protocol;

public:
    void run() override;
    //Reciever(Queue<Mensaje>& queue, Protocol& protocol, ClientesMonitor& clientes,
    //         std::atomic<bool>& is_alive);
};

#endif
