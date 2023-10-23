#ifndef ACEPTADOR_HEADER
#define ACEPTADOR_HEADER

#include "socket.h"
#include "thread.h"

class Recibidor : public Thread {
    Socket socket;

public:

    Recibidor(const char *puerto);

    // Devuelve el socket del cliente entrante
    [[nodiscard]] Socket esperarCliente();

    void run();
    
  

};



#endif
