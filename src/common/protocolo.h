#ifndef PROTOCOLO_HEADER
#define PROTOCOLO_HEADER

#include "socket.h"
#include "threadSafeList.h"
#include "defs.h"
#include <arpa/inet.h>
#include <string>
#include <vector>


class Protocolo {
private:
    Socket socket;

    id obtenerId();
    int8_t obtenerCodigo();
    std::vector<id> obtenerVector();
    id verificarConexion();
    bool enviarCodigo(int codigo);
    bool enviarCantidad(int cant);
    bool enviarId(id id);
public:

    Protocolo(Socket&& socket);

#ifdef CLIENT
    // METODOS DEL CLIENTE
    bool pedirInformacion(tipoInfo infoAPedir);
    std::vector<RepresentacionMapa> obtenerMapas();
    std::vector<id> obtenerPartidas();
    id crearPartida(id mapaSeleccionado);
    bool unirseAPartida(id id);
    bool moverGusano(id gusano, Direccion direccion);
    std::vector<int> recibirEstadoDelJuego();
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
    bool enviarEstadoDelJuego(std::vector<int> estado);
#endif


};

#endif
