#ifndef PROTOCOLO_HEADER
#define PROTOCOLO_HEADER

#include "socket.h"
#include "threadSafeList.h"
#include <arpa/inet.h>
#include <string>
#include <vector>

//TODO Poner todos los types defs en un header file juntos
typedef int32_t id;
enum Direccion {IZQUIERDA, DERECHA, SALTO, PIRUETA};
enum tipoInfo {PARTIDA, MAPA};
#define PARTIDAS 1
#define MAPAS 2
#define CREAR 3
#define UNIRSE 4
#define PEDIDO 5
#define EXITO 6
#define ERROR 7

// Codigos para acciones 
// mov + direccion -> izq, der, salto, pirueta
#define MOV 8
#define noIgn [[nodiscard]]

struct RepresentacionPartida {
    id ID;
};

class Protocolo {
private:
    Socket socket;

    id obtenerId();
    int8_t obtenerCodigo();
    std::vector<id> obtenerVector();
    id verificarConexion();
public:

    Protocolo(Socket&& socket);

#ifdef CLIENT
    // METODOS DEL CLIENTE
    bool pedirInformacion(tipoInfo infoAPedir);
    std::vector<id> obtenerMapas();
    std::vector<id> obtenerPartidas();
    id crearPartida(id mapaSeleccionado);
    bool unirseAPartida(id id);
    bool moverGusano(id gusano, Direccion direccion);
#endif

#ifdef SERVER
    // METODOS DEL SERVER
    noIgn tipoInfo obtenerPedido();
    bool enviarMapas(std::vector<std::string> mapasDisponibles);
    bool enviarPartidas(std::vector<RepresentacionPartida> partidasDisponibles); 
    noIgn id obtenerMapaDeseado();
    noIgn id obtenerPartidaDeseada();

    bool enviarConfirmacion(id idPartida);
    bool enviarError();

    Direccion obtenerAccion();
#endif


};

#endif
