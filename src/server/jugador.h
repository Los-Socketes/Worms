#ifndef JUGADOR_HEADER
#define JUGADOR_HEADER

#include "defs.h"
#include "gusano.h"
#include "protocolo.h"
#include "queue.h"
#include "reciever.h"
#include "sender.h"
#include "socket.h"
#include <vector>

//Forward declaration por include circulares
class MonitorPartida;

class Jugador {
private:
    Protocolo protocolo;
    Sender enviador;
    Reciever recibidor;

    int gusanoActualPos;
    std::vector<Gusano*> gusanos;

    //TODO Esto seria de accion en el futuro. O anadir otra queue (eh)
    //o cambiar
    Queue<Direccion>* acciones;

public:
    Jugador(Socket&& socket, std::vector<std::string> mapasDisponibles,
	       MonitorPartida& partidasDisponibles);
    Gusano* getGusanoActual();

    void obtenerGusanosIniciales(std::vector<Gusano*> gusanos);

    void obtenerAccesoAAcciones(Queue<Direccion>* acciones);

    bool obtenerAccion(Direccion& primeraAccion);

    void enviarEstadoJuego(EstadoDelJuego estado);
};

#endif
