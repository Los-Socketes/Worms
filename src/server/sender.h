#ifndef SENDER_H
#define SENDER_H

#include <utility>

#include "protocol.h"
#include "queue.h"
#include "thread.h"

class Sender: public Thread {
    Queue<Mensaje>& queue;
    Protocol& protocol;

public:
    void run() override;
    //Sender(Queue<Mensaje>& msg_queue, Protocol& protocol_ref, std::atomic<bool>& is_alive);
};

#endif
