// Se le pide que cree un main a catch
#define CATCH_CONFIG_MAIN 
#include "protocolo.h"
#include "socket.h"
#include <iostream>

#if DISTRO == 1
#include <catch2/catch_test_macros.hpp>
#elif DISTRO == 2
#include <catch2/catch.hpp>
#endif

#include <cstdint>

// Inicializo las variables que voy a usar en los tests
static Socket mockSocketServer("9097");
static Socket mockSocket("127.0.0.1", "9097");
static Protocolo protocolo(std::move(mockSocket));
static Socket peer = mockSocketServer.accept();
static Protocolo protocoloServer(std::move(peer));

// TEST 1
tipoInfo pedirInformacion(tipoInfo info) {
    protocolo.pedirInformacion(info);
    return protocoloServer.obtenerPedido();
}

TEST_CASE( "Tests de pedirInforamcion", "[pedirInformacion]" ) {
    REQUIRE( pedirInformacion(MAPA) == MAPA);
    REQUIRE( pedirInformacion(PARTIDA) == PARTIDA);
}


// // TEST 2
Accion moverGusano(id gusano, Direccion dir) {
    protocolo.moverGusano(gusano, dir);
    return protocoloServer.obtenerAccion();
}

TEST_CASE( "Tests de mover gusano", "[moverGusano]" ) {
    Direccion moverseA = INICIO_DER;
    Accion moverseDerecha;
    moverseDerecha.idGusano = (id)0;
    moverseDerecha.accion = MOVERSE;
    moverseDerecha.dir = moverseA;

    Accion resultado = moverGusano((id)0, moverseA);
    REQUIRE(resultado.idGusano == moverseDerecha.idGusano);
    REQUIRE(resultado.accion == moverseDerecha.accion);
    REQUIRE(resultado.dir == moverseDerecha.dir);

    moverseA = INICIO_IZQ;
    Accion moverseIzquierda;
    moverseIzquierda.idGusano = (id)1;
    moverseIzquierda.accion = MOVERSE;
    moverseIzquierda.dir = moverseA;
    resultado = moverGusano((id)1, moverseA);
    REQUIRE(resultado.idGusano == moverseIzquierda.idGusano);
    REQUIRE(resultado.accion == moverseIzquierda.accion);
    REQUIRE(resultado.dir == moverseIzquierda.dir);
}

// TEST 3

std::vector<std::string> enviarMapasStrings(std::vector<std::string> mapas) {
    protocoloServer.enviarMapas(mapas);
    std::vector<RepresentacionMapa> repMapas = protocolo.obtenerMapas();
    std::vector<std::string> listaMapas;
    for (auto &&mapa : repMapas) {
        listaMapas.push_back(mapa.nombre);
    }
    
    return listaMapas;
}

TEST_CASE( "Tests de enviar mapas nombres", "[enviarMapasStrings]" ) {
    std::vector<std::string> mapas = {"mapa1", "mapa2"};
    REQUIRE( enviarMapasStrings(mapas) == mapas);
}

// TEST 4

std::vector<id> enviarMapasIds(std::vector<std::string> mapas) {
    protocoloServer.enviarMapas(mapas);
    std::vector<RepresentacionMapa> repMapas = protocolo.obtenerMapas();
    std::vector<id> listaMapas;
    for (auto &&mapa : repMapas) {
        listaMapas.push_back(mapa.ID);
    }
    return listaMapas;
}

TEST_CASE( "Tests de enviar mapas ids", "[enviarMapasIds]" ) {
    std::vector<std::string> mapas = {"mapa1", "mapa2"};
    std::vector<id> ids = {(id)0, (id)1};
    REQUIRE( enviarMapasIds(mapas) == ids);
}

// TEST 5

std::vector<id> enviarPartidas(std::vector<RepresentacionPartida> partidas) {
    protocoloServer.enviarPartidas(partidas);
    return protocolo.obtenerPartidas();
}

TEST_CASE( "Tests de enviar partidas", "[enviarPartidas]" ) {
    RepresentacionPartida partida1;
    partida1.ID = 1;
    std::vector<RepresentacionPartida> partidas = {partida1};
    std::vector<id> ids = {partida1.ID};
    REQUIRE( enviarPartidas(partidas) == ids);
}

// TEST 6

// EstadoDelJuego enviarEstadoDelJuego(EstadoDelJuego estado) {
//     protocoloServer.enviarEstadoDelJuego(estado);
//     return protocolo.recibirEstadoDelJuego();
// }

// TEST_CASE( "Tests de enviar estado del Juego", "[enviarEstadoDelJuego]" ) {
//     EstadoDelJuego estado1;
//     estado1.dir = DERECHA;
//     std::pair<int,int> posicion1(1,2);
//     estado1.posicion = posicion1;
//     REQUIRE( enviarEstadoDelJuego(estado1).posicion == estado1.posicion);
//     REQUIRE( enviarEstadoDelJuego(estado1).dir == estado1.dir);

//     estado1.dir = IZQUIERDA;
//     REQUIRE( enviarEstadoDelJuego(estado1).posicion == estado1.posicion);
//     REQUIRE( enviarEstadoDelJuego(estado1).dir == estado1.dir);

//     std::pair<int,int> posicion2(20,50);
//     estado1.posicion = posicion2;
//     REQUIRE( enviarEstadoDelJuego(estado1).posicion == estado1.posicion);
//     REQUIRE( enviarEstadoDelJuego(estado1).dir == estado1.dir);
// }
