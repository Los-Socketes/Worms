#ifndef SERVER_HEADER
#define SERVER_HEADER

#include "recibidor.h"
#include "socket.h"

#include "cliente.h"

//Hago la declaracion para que compile. Cuando este implentado borro
//esto

class Server {
private:
    // Recibidor recibidor;
    Socket aceptador;
    
    void recibirCliente();

public:
    Server(const char *puerto);
};



#endif
