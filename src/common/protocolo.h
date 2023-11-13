#ifndef PROTOCOLO_HEADER
#define PROTOCOLO_HEADER

#include "socket.h"
#include "TSList.h"
#include "defs.h"
#include <arpa/inet.h>
#include <string>
#include <vector>
#include <memory>


class Protocolo {
private:
    Socket socket;
    int maxYEnMapa;

    id obtenerId();
    int8_t obtenerCodigo();
    std::vector<id> obtenerVector();
    id verificarConexion();
    bool enviarCodigo(int codigo);
    bool enviarCantidad(int cant);
    bool enviarId(id id);

    float toFloat(int valor);
    int toInt(float valor);
public:

    Protocolo(Socket&& socket);
    void cerrarConexion(int forma);

#ifdef CLIENT
    // METODOS DEL CLIENTE
    bool pedirInformacion(tipoInfo infoAPedir);
    std::vector<RepresentacionMapa> obtenerMapas();
    std::vector<id> obtenerPartidas();
    id crearPartida(id mapaSeleccionado);
    bool unirseAPartida(id idPartida);
    bool moverGusano(Direccion direccion);
    bool equiparArma(ArmaProtocolo arma);
    bool configurarAngulo(float angulo);
    bool configurarPotencia(float potencia);
    bool configurarCuentaRegresiva(int valor);
    bool atacar();
    std::shared_ptr<EstadoDelJuego> obtenerEstadoDelJuego();

    void setMaxY(int y);
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

    Accion obtenerAccion();
    // TODO: enviar representacion arma
    bool enviarEstadoDelJuego(std::shared_ptr<EstadoDelJuego> estado);
#endif


};

#endif
