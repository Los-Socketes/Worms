#ifndef PROTOCOLO_HEADER
#define PROTOCOLO_HEADER

#include "socket.h"

#include <arpa/inet.h>
#include <string>
#include <vector>

//TODO Poner todos los types defs en un header file juntos
typedef int32_t id;
enum Direccion {IZQUIERDA, DERECHA, SALTO, PIRUETA};
#define PARTIDAS 1
#define MAPAS 2
#define CREAR 3
#define CREADA 4
#define UNIRSE 5

// Codigos para acciones 
// mov + direccion -> izq, der, salto, pirueta
// 
#define MOV 6

class Protocolo {
private:
    Socket socket;

    id obtenerId();
    int8_t obtenerCodigo();
    std::vector<id> obtenerVector();
    std::vector<char*> vectorListoParaEnviar(std::vector<id> vectorAEnviar);
public:
    // Pongo estas para que compile nomas. Ponele el nombre/encaralas de
    // la forma que te parezca
    std::vector<id> obtenerPartidas();
    std::vector<id> obtenerMapas();
    id crearPartida(id mapaSeleccionado);
    bool unirseAPartida(id id);
    void moverGusano(id gusano, Direccion direccion);

    void enviarMapas(std::vector<id> mapasDisponibles); 

    void enviarPartidas(std::vector<id> partidasDisponibles); 
    id obtenerMapaDeseado();
    [[nodiscard]] id obtenerPartidaDeseada();

    Protocolo(Socket&& socket);

};

#endif
