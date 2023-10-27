#ifndef SENDER_H
#define SENDER_H

#include <utility>

#include "protocolo.h"
#include "queue.h"
#include "thread.h"

class Sender: public Thread {
    // Queue<Mensaje>& queue;
    Protocolo& protocol;

public:
    void run() override;
    //Sender(Queue<Mensaje>& msg_queue, Protocol& protocol_ref, std::atomic<bool>& is_alive);
    Sender(Protocolo& protocolo);
};

#endif
