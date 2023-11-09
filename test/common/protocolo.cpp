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

// TEST 2
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

std::pair<id,id> crearPartidaCasoFeliz(id mapa, id partida) {
    protocoloServer.enviarConfirmacion(partida);
    id idPartida = protocolo.crearPartida(mapa);
    id idMapa = protocoloServer.obtenerMapaDeseado();
    std::pair<id,id> ids(idPartida, idMapa);
    return ids;
}

TEST_CASE("Test de crear partidas (Caso feliz)", "[crearPartidaCasoFeliz]") {
    std::pair<id,id> resultado = crearPartidaCasoFeliz((id)0, (id)1);
    REQUIRE( resultado.first == (id)1);
    REQUIRE( resultado.second == (id)0);
}

// TEST 7

std::pair<id,id> crearPartidaCasoError(id mapa) {
    protocoloServer.enviarError();
    id idPartida = protocolo.crearPartida(mapa);
    id idMapa = protocoloServer.obtenerMapaDeseado();
    std::pair<id,id> ids(idPartida, idMapa);
    return ids;
}

TEST_CASE("Test de crear partidas (Caso error)", "[crearPartidaCasoError]") {
    std::pair<id,id> resultado = crearPartidaCasoError((id)0);
    REQUIRE(resultado.first == INVAL_ID);
    REQUIRE(resultado.second == (id)0);
}

// TEST 8

std::pair<bool,id> unirsePartidaCasoFeliz(id partida) {
    protocoloServer.enviarConfirmacion((id)0);
    bool res = protocolo.unirseAPartida(partida);
    id idPartida = protocoloServer.obtenerPartidaDeseada();
    std::pair<bool,id> resultado(res, idPartida);
    return resultado;
}

TEST_CASE("Test de unirse a partidas (Caso feliz)", "[unirsePartidaCasoFeliz]") {
    std::pair<bool,id> resultado = unirsePartidaCasoFeliz((id)0);
    REQUIRE(resultado.first);
    REQUIRE(resultado.second == (id)0);
}

// TEST 9

std::pair<bool,id> unirsePartidaCasoError(id partida) {
    protocoloServer.enviarError();
    bool res = protocolo.unirseAPartida(partida);
    id idPartida = protocoloServer.obtenerPartidaDeseada();
    std::pair<bool,id> resultado(res, idPartida);
    return resultado;
}

TEST_CASE("Test de unirse a partidas (Caso error)", "[unirsePartidaCasoError]") {
    std::pair<bool,id> resultado = unirsePartidaCasoError((id)0);
    REQUIRE(!resultado.first);
    REQUIRE(resultado.second == (id)0);
}

// TEST 10

Accion obtenerAccionDeEquipar(id gusano, ArmaProtocolo arma) {
    protocolo.equiparArma(gusano, arma);
    return protocoloServer.obtenerAccion();
}

TEST_CASE( "Tests de equipar arma a gusano", "[obtenerAccionDeEquipar]" ) {
    ArmaProtocolo armaAEquipar = BATE_P;
    id idGusano = 0;
    Accion equipar;
    equipar.idGusano = idGusano;
    equipar.accion = EQUIPARSE;
    equipar.armaAEquipar = armaAEquipar;
    Accion resultado = obtenerAccionDeEquipar(idGusano, armaAEquipar);
    REQUIRE(resultado.idGusano == equipar.idGusano);
    REQUIRE(resultado.accion == equipar.accion);
    REQUIRE(resultado.armaAEquipar == equipar.armaAEquipar);

    armaAEquipar = BAZOOKA_P;
    idGusano = 1;
    equipar.idGusano = idGusano;
    equipar.accion = EQUIPARSE;
    equipar.armaAEquipar = armaAEquipar;
    resultado = obtenerAccionDeEquipar(idGusano, armaAEquipar);
    REQUIRE(resultado.idGusano == equipar.idGusano);
    REQUIRE(resultado.accion == equipar.accion);
    REQUIRE(resultado.armaAEquipar == equipar.armaAEquipar);
}

// TEST 11

Accion obtenerAccionDeAtacar(id gusano) {
    protocolo.atacar(gusano);
    return protocoloServer.obtenerAccion();
}

TEST_CASE( "Tests de atacar", "[obtenerAccionDeAtacar]" ) {
    id idGusano = 0;
    Accion atacar;
    atacar.idGusano = idGusano;
    atacar.accion = ATAQUE;

    Accion resultado = obtenerAccionDeAtacar(idGusano);
    REQUIRE(resultado.idGusano == atacar.idGusano);
    REQUIRE(resultado.accion == atacar.accion);

    idGusano = 1;
    atacar.idGusano = idGusano;
    atacar.accion = ATAQUE;
    resultado = obtenerAccionDeAtacar(idGusano);
    REQUIRE(resultado.idGusano == atacar.idGusano);
    REQUIRE(resultado.accion == atacar.accion);
}

// TEST 12

EstadoDelJuego enviarEstadoDelJuego(EstadoDelJuego estado) {
    protocolo.setMaxY(0);
    protocoloServer.enviarEstadoDelJuego(estado);
    return protocolo.obtenerEstadoDelJuego();
}

TEST_CASE( "Tests de enviar estado del Juego", "[enviarEstadoDelJuego]" ) {
    idJugador jugador = 0;
    EstadoDelJuego estado1;
    std::map<idJugador, std::vector<RepresentacionGusano>> gusanos;
    RepresentacionGusano gusano1;
    gusano1.idGusano = (id)2;
    gusano1.vida = (hp)20;
    gusano1.dir = DERECHA;
    std::pair<coordX, coordY> posicion1(2.0345, 5.6789);
    gusano1.posicion = posicion1;
    gusano1.armaEquipada = NADA_P;
    
    RepresentacionGusano gusano2;
    gusano2.idGusano = (id)3;
    gusano2.vida = (hp)25;
    gusano2.dir = DERECHA;
    std::pair<coordX, coordY> posicion2(2.0345, 5.6789);
    gusano2.posicion = posicion2;
    gusano2.armaEquipada = GRANADA_VERDE_P;
    std::vector<RepresentacionGusano> listaGusanos;
    listaGusanos.push_back(gusano1);
    listaGusanos.push_back(gusano2);
    gusanos.insert({jugador, listaGusanos});
    estado1.gusanos = gusanos;
    
    EstadoDelJuego resultado = enviarEstadoDelJuego(estado1);
    REQUIRE(resultado.gusanos.size() == estado1.gusanos.size());
    std::vector<RepresentacionGusano> resultadoGusanos = resultado.gusanos[jugador];
    REQUIRE(resultadoGusanos.size() == listaGusanos.size());
    
    for (int i = 0; i < (int)resultadoGusanos.size(); i++) {
        RepresentacionGusano resultadoGusano = resultadoGusanos[i];
        RepresentacionGusano baseGusano = listaGusanos[i];
        REQUIRE(resultadoGusano.vida == baseGusano.vida);
        REQUIRE(resultadoGusano.idGusano == baseGusano.idGusano);
        REQUIRE(resultadoGusano.dir == baseGusano.dir);
        REQUIRE(resultadoGusano.posicion.first == baseGusano.posicion.first);
        REQUIRE(resultadoGusano.posicion.second == -baseGusano.posicion.second);
        REQUIRE(resultadoGusano.armaEquipada == baseGusano.armaEquipada);

    }


    

    
}
