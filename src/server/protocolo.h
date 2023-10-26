#ifndef PROTOCOLO_HEADER
#define PROTOCOLO_HEADER

#include "socket.h"
#include "partida.h"
#include <arpa/inet.h>
#include <string>
#include <vector>

//TODO Poner todos los types defs en un header file juntos
typedef int32_t id;
enum Direccion {IZQUIERDA, DERECHA, SALTO, PIRUETA};
#define PARTIDAS 1
#define MAPAS 2
#define CREAR 3
#define UNIRSE 4
#define EXITO 5
#define ERROR 6

// Codigos para acciones 
// mov + direccion -> izq, der, salto, pirueta
// 
#define MOV 7

class Protocolo {
private:
    Socket socket;

    id obtenerId();
    int8_t obtenerCodigo();
    std::vector<id> obtenerVector();
    std::vector<char*> vectorListoParaEnviar(std::vector<id> vectorAEnviar);
    id verificarConexion();
public:
    // Pongo estas para que compile nomas. Ponele el nombre/encaralas de
    // la forma que te parezca
    std::vector<id> obtenerPartidas();
    std::vector<id> obtenerMapas();
    id crearPartida(id mapaSeleccionado);
    bool unirseAPartida(id id);
    void moverGusano(id gusano, Direccion direccion);

    void enviarMapas(std::vector<std::string> mapasDisponibles)

    void enviarPartidas(std::vector<Partida*> partidasDisponibles); 
    id obtenerMapaDeseado();
    [[nodiscard]] id obtenerPartidaDeseada();

    void enviarConfirmacion(id idPartida);
    void enviarError();

    Protocolo(Socket&& socket);

};

#endif
