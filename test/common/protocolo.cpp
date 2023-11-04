#include "protocolo.h"
#include "socket.h"
#include <iostream>

#include <catch2/catch.hpp>

#include <cstdint>

tipoInfo pedirInformacion(tipoInfo info) {
    Socket mockSocketServer("9097");
    Socket mockSocket("127.0.0.1", "9097");
    Protocolo protocolo(std::move(mockSocket));
    Socket accept = mockSocketServer.accept();
    Protocolo protocoloServer(std::move(accept));

    protocolo.pedirInformacion(info);
    return protocoloServer.obtenerPedido();
}

TEST_CASE( "Tests de pedirInforamcion", "[pedirInformacion]" ) {
    REQUIRE( pedirInformacion(MAPA) == MAPA);
    REQUIRE( pedirInformacion(PARTIDA) == PARTIDA);
}


// int main() {
//     Socket mockSocketServer("9097");
//     Socket mockSocket("127.0.0.1", "9097");
//     Protocolo protocolo(std::move(mockSocket));
//     Socket accept = mockSocketServer.accept();
//     Protocolo protocoloServer(std::move(accept));

//     std::cout << "Inicio test de protocolo\n";



//     // TESTEO PEDIR INFORMACION
//     protocolo.pedirInformacion(MAPA);
//     tipoInfo recibido = protocoloServer.obtenerPedido();
//     if (recibido != MAPA) {
//         std::cerr << recibido;
//         return 1;
//     }

//     protocolo.pedirInformacion(PARTIDA);
//     recibido = protocoloServer.obtenerPedido();
//     if (recibido != PARTIDA) {
//         std::cerr << recibido;
//         return 1;
//     }
//     return 0;
// }
