#ifndef ACEPTADOR_HEADER
#define ACEPTADOR_HEADER

#include "jugador.h"
#include "socket.h"
#include "thread.h"
#include <vector>

#define aceptarClientes run

class Aceptador : public Thread {
    Socket socket;

    //TODO: Hacerlo un puntero
    const std::vector<std::string> escenariosDisponibles;

    MonitorPartida& partidas;

 public:
    Aceptador(const char *puerto,
	    std::vector<std::string> escenariosDisponibles,
	    MonitorPartida &partidas);

    void aceptarClientes();
};

#endif
