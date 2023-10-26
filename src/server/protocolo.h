#ifndef PROTOCOLO_HEADER
#define PROTOCOLO_HEADER

#include "socket.h"
#include <string>
#include <vector>

//TODO Poner todos los types defs en un header file juntos

class Protocolo {
private:
    Socket socket;
public:
    // Pongo estas para que compile nomas. Ponele el nombre/encaralas de
    // la forma que te parezca
    id_t crearPartida(std::string mapaSeleccionado);
    bool unirseAPartida(id_t id);
    void moverGusano(id_t gusano, Direccion direccion);

    void enviarMapas(std::vector<std::string> mapasDisponibles); 

    void enviarMapas(std::vector<std::string> partidasDisponibles); 

    [[nodiscard]] int obtenerPartidaDeseada();

    Protocolo(Socket&& socket);

};

#endif
