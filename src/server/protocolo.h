#ifndef PROTOCOLO_HEADER
#define PROTOCOLO_HEADER

#include "socket.h"

#include <arpa/inet.h>
#include <string>
#include <vector>

//TODO Poner todos los types defs en un header file juntos
enum Direccion {IZQ, DER};
#define PARTIDAS 1
#define MAPAS 2
#define ACCION 3
#define CREAR 4
#define CREADA 5
#define UNIRSE 6

class Protocolo {
private:
    Socket socket;

    std::vector<int32_t> obtenerVector();
    std::vector<char*> vectorListoParaEnviar(std::vector<int32_t> vectorAEnviar);
public:
    // Pongo estas para que compile nomas. Ponele el nombre/encaralas de
    // la forma que te parezca
    std::vector<int32_t> obtenerPartidas();
    std::vector<int32_t> obtenerMapas();
    int32_t crearPartida(int32_t mapaSeleccionado);
    bool unirseAPartida(int32_t id);
    void moverGusano(int32_t gusano, Direccion direccion);

    void enviarMapas(std::vector<int32_t> mapasDisponibles); 

    void enviarPartidas(std::vector<int32_t> partidasDisponibles); 

    [[nodiscard]] int32_t obtenerPartidaDeseada();

    Protocolo(Socket&& socket);

};

#endif
