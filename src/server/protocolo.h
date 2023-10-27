#include <vector>
#ifndef PROTOCOLO_HEADER
#define PROTOCOLO_HEADER

#include "socket.h"
// #include "partida.h"
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

    // METODOS DEL CLIENTE
    void pedirInformacion(tipoInfo infoAPedir);
    std::vector<id> obtenerPartidas();
    std::vector<id> obtenerMapas();
    id crearPartida(id mapaSeleccionado);
    bool unirseAPartida(id id);
    void moverGusano(id gusano, Direccion direccion);


    // METODOS DEL SERVER
    tipoInfo obtenerPedido();
    void enviarMapas(std::vector<std::string> mapasDisponibles);
    void enviarPartidas(std::vector<RepresentacionPartida> partidasDisponibles); 
    id obtenerMapaDeseado();
    [[nodiscard]] id obtenerPartidaDeseada();

    void enviarConfirmacion(id idPartida);
    void enviarError();

    Direccion recibirAccion();

    Protocolo(Socket&& socket);

};

#endif
