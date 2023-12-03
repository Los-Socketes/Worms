#ifndef ACEPTADOR_HEADER
#define ACEPTADOR_HEADER

#include "cliente.h"
#include "socket.h"
#include "thread.h"
#include "monitorPartida.h"
#include <vector>
#include <list>

#define aceptarClientes run

class Aceptador : public Thread {
    Socket socket;

    Mapas escenariosDisponibles;

    MonitorPartida partidas;
    std::list<Cliente*> listaClientes;

 public:
    Aceptador(const char *puerto);

    void aceptarClientes();

    void reap_dead();
    void kill();
};

#endif
