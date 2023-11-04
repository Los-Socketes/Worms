// Se le pide que cree un main a catch
#define CATCH_CONFIG_MAIN 
#include "protocolo.h"
#include "socket.h"
#include <iostream>

#include <catch2/catch.hpp>

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
Direccion moverGusano(id gusano, Direccion dir) {
    protocolo.moverGusano(gusano, dir);
    return protocoloServer.obtenerAccion();
}

TEST_CASE( "Tests de mover gusano", "[moverGusano]" ) {
    REQUIRE( moverGusano((id)0, INICIO_DER) == INICIO_DER);
    REQUIRE( moverGusano((id)1, INICIO_IZQ) == INICIO_IZQ);
}

