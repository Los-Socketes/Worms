#ifndef RECIEVER_H
#define RECIEVER_H

#include <utility>

#include "defs.h"
#include "protocolo.h"
#include "queue.h"
#include "thread.h"
#include "mapa.h"
// #include "monitorPartida.h"

//Forward declaration
class MonitorPartida;
class Cliente;


#define recibirEstado run

class Reciever: public Thread {
    // Queue<Mensaje>& queue;
    Protocolo& protocolo;

    Queue<Accion> *acciones;
    Mapas mapasDisponibles;
    MonitorPartida& partidas;
    Cliente *cliente;
    idJugador miId;

    bool lobby();

    void recibirEstado() override;

public:

    Reciever(Protocolo& protocol, Mapas mapasDisponibles, MonitorPartida& monitorPartidas, Cliente *cliente);

    void obtener(Queue<Accion> *acciones);

};

#endif
