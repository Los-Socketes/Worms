#ifndef ACEPTADOR_HEADER
#define ACEPTADOR_HEADER

#include "socket.h"
#include "thread.h"

#define aceptarClientes run

class Aceptador : public Thread {
    Socket socket;


 public:
    Aceptador(const char *puerto);

    void aceptarClientes();
};

#endif
