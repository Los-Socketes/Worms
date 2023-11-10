#ifndef CLIENTE_HEADER
#define CLIENTE_HEADER

#include "protocolo.h"
#include "sender.h"
#include "reciever.h"

class MonitorPartida;

class Cliente {
private:
    Protocolo protocolo;
    Sender sender;
    Reciever reciever;


public:
    Cliente(Socket&& socket, strings mapasDisponibles, MonitorPartida& monitorPartidas);

    void obtenerAccesoAAcciones(Queue<Accion>* acciones);

    void enviarEstadoJuego(EstadoDelJuego estado);

    bool estaMuerto();

    void cerrarSocket(int forma);

    ~Cliente();

};

#endif
