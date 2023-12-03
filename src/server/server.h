#ifndef SERVER_HEADER
#define SERVER_HEADER

#include "aceptador.h"
#include <string>

class Server {
private:
    Aceptador aceptador;

public:
    Server(const char *puerto);

    void esperarQueSeCierre();

};



#endif
