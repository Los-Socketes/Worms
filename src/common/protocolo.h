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

    id obtenerId();
    int8_t obtenerCodigo();
    std::vector<id> obtenerVector();
    InformacionInicial verificarConexion();
    bool enviarCodigo(int codigo);
    bool enviarCantidad(int cant);
    bool enviarId(id id);

    float toFloat(int valor);
    int toInt(float valor);

#ifdef CLIENT
    std::map<idJugador, SituacionJugador> obtenerSituacion(int cantJugadores);
    std::map<idJugador, std::map<id, RepresentacionGusano>> obtenerGusanos(int cantJugadores);
    std::vector<RepresentacionProyectil> obtenerProyectiles();
    std::vector<RepresentacionProvisiones> obtenerProvisiones();
#endif

#ifdef SERVER
    bool enviarProvisiones(std::vector<RepresentacionProvisiones> provisiones);
    bool enviarProyectiles(std::vector<RepresentacionProyectil> proyectiles);
    bool enviarGusanos(std::map<idJugador, std::map<id, RepresentacionGusano>> gusanos);
    bool enviarSituacion(std::map<idJugador, SituacionJugador> situacionJugadores);
#endif

public:

    Protocolo(Socket&& socket);
    void cerrarConexion(int forma);

#ifdef CLIENT
    // METODOS DEL CLIENTE
    bool pedirInformacion(tipoInfo infoAPedir);
    std::vector<RepresentacionMapa> obtenerMapas();
    std::vector<id> obtenerPartidas();
    InformacionInicial crearPartida(id mapaSeleccionado);
    InformacionInicial unirseAPartida(id idPartida);
    bool iniciarPartida();
    bool moverGusano(Direccion direccion);
    bool equiparArma(ArmaProtocolo arma);
    bool configurarAngulo(float angulo);
    bool configurarPotencia(float potencia);
    bool configurarCuentaRegresiva(int valor);
    bool configurarCoordenadas(std::pair<coordX,coordY> coordenadas);
    bool enviarCheat(TipoCheat cheat);
    bool atacar();
    std::shared_ptr<EstadoDelJuego> obtenerEstadoDelJuego();
#endif

#ifdef SERVER
    // METODOS DEL SERVER
    noIgn tipoInfo obtenerPedido();
    bool enviarMapas(std::vector<std::string> mapasDisponibles);
    bool enviarPartidas(std::vector<RepresentacionPartida> partidasDisponibles); 
    noIgn id obtenerMapaDeseado();
    noIgn id obtenerPartidaDeseada();

    bool enviarConfirmacion(InformacionInicial idPartida);
    bool enviarError();

    Accion obtenerAccion();
    // TODO: enviar representacion arma
    bool enviarEstadoDelJuego(std::shared_ptr<EstadoDelJuego> estado);
#endif


};

#endif
