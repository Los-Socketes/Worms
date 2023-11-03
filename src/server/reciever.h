#ifndef RECIEVER_H
#define RECIEVER_H

#include <utility>

#include "protocolo.h"
#include "queue.h"
#include "thread.h"

class Reciever: public Thread {
    // Queue<Mensaje>& queue;
    Protocolo& protocolo;

    Queue<Direccion> *acciones;

public:
    void run() override;
    //Reciever(Queue<Mensaje>& queue, Protocol& protocol, ClientesMonitor& clientes,
    //         std::atomic<bool>& is_alive);
    // 		                                  ;)
    // Reciever(Protocolo& protocol, Queue<Direccion> *acciones);
    Reciever(Protocolo& protocol);

    void darAcceso(Queue<Direccion> *acciones);
    // noIgn id obtenerMapaDeseado();

    // noIgn tipoInfo obtenerPedido();
};

#endif
