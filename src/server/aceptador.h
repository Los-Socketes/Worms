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
    std::vector<std::string> escenariosDisponibles;

    MonitorPartida& partidas;

 public:
    Aceptador(const char *puerto,
	    MonitorPartida &partidas);
    void asignar(
	       std::vector<std::string> escenariosDisponibles);

    void aceptarClientes();
};

#endif
