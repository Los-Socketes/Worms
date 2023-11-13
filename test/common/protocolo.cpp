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
Accion moverGusano(Direccion dir) {
    protocolo.moverGusano(dir);
    return protocoloServer.obtenerAccion();
}

TEST_CASE( "Tests de mover gusano", "[moverGusano]" ) {
    Direccion moverseA = INICIO_DER;
    Accion moverseDerecha;
    moverseDerecha.accion = MOVERSE;
    moverseDerecha.dir = moverseA;

    Accion resultado = moverGusano(moverseA);
    REQUIRE(resultado.accion == moverseDerecha.accion);
    REQUIRE(resultado.dir == moverseDerecha.dir);

    moverseA = INICIO_IZQ;
    Accion moverseIzquierda;
    moverseIzquierda.accion = MOVERSE;
    moverseIzquierda.dir = moverseA;
    resultado = moverGusano(moverseA);
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

Accion obtenerAccionDeEquipar(ArmaProtocolo arma) {
    protocolo.equiparArma(arma);
    return protocoloServer.obtenerAccion();
}

TEST_CASE( "Tests de equipar arma a gusano", "[obtenerAccionDeEquipar]" ) {
    ArmaProtocolo armaAEquipar = BATE_P;
    Accion equipar;
    equipar.accion = EQUIPARSE;
    equipar.armaAEquipar = armaAEquipar;
    Accion resultado = obtenerAccionDeEquipar(armaAEquipar);
    REQUIRE(resultado.accion == equipar.accion);
    REQUIRE(resultado.armaAEquipar == equipar.armaAEquipar);

    armaAEquipar = BAZOOKA_P;
    equipar.accion = EQUIPARSE;
    equipar.armaAEquipar = armaAEquipar;
    resultado = obtenerAccionDeEquipar(armaAEquipar);
    REQUIRE(resultado.accion == equipar.accion);
    REQUIRE(resultado.armaAEquipar == equipar.armaAEquipar);
}

// TEST 11

Accion obtenerAccionDeAtacar() {
    protocolo.atacar();
    return protocoloServer.obtenerAccion();
}

TEST_CASE( "Tests de atacar", "[obtenerAccionDeAtacar]" ) {
    Accion atacar;
    atacar.accion = ATAQUE;

    Accion resultado = obtenerAccionDeAtacar();
    REQUIRE(resultado.accion == atacar.accion);
}

// TEST 12

std::shared_ptr<EstadoDelJuego> enviarEstadoDelJuego(std::shared_ptr<EstadoDelJuego> estado) {
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
    gusano1.estado = SALTANDO;
    std::pair<coordX, coordY> posicion1(2.0345, 5.6789);
    gusano1.posicion = posicion1;
    RepresentacionArma arma1;
    arma1.tieneMira = true;
    arma1.tienePotenciaVariable = true;
    arma1.tieneCuentaRegresiva = false;
    arma1.municiones = -1;
    arma1.fragmentos = 0;
    arma1.danio.epicentro = 50;
    arma1.danio.radio = 2;
    arma1.anguloRad = 0;
    arma1.potencia = 1;
    arma1.cuentaRegresiva = 0;
    arma1.arma = BAZOOKA_P;

    gusano1.armaEquipada = arma1;
    
    RepresentacionGusano gusano2;
    gusano2.idGusano = (id)3;
    gusano2.vida = (hp)25;
    gusano2.dir = DERECHA;
    gusano2.estado = CAYENDO;
    std::pair<coordX, coordY> posicion2(2.0345, 5.6789);
    gusano2.posicion = posicion2;

    RepresentacionArma arma2;
    arma2.tieneMira = true;
    arma2.tienePotenciaVariable = true;
    arma2.tieneCuentaRegresiva = true;
    arma2.municiones = 10;
    arma2.fragmentos = 6;
    arma2.danio.epicentro = 30;
    arma2.danio.radio = 2;
    arma2.danioFragmento.epicentro = 10;
    arma2.danioFragmento.radio = 2;
    arma2.anguloRad = 0;
    arma2.potencia = 1;
    arma2.cuentaRegresiva = 0;
    arma2.arma = GRANADA_ROJA_P;
    gusano2.armaEquipada = arma2;
    std::vector<RepresentacionGusano> listaGusanos;
    listaGusanos.push_back(gusano1);
    listaGusanos.push_back(gusano2);
    gusanos.insert({jugador, listaGusanos});
    estado1.gusanos = gusanos;

    std::shared_ptr<EstadoDelJuego> estado(new EstadoDelJuego);
    estado->gusanos = estado1.gusanos;
    
    std::shared_ptr<EstadoDelJuego> resultado = enviarEstadoDelJuego(estado);
    REQUIRE(resultado->gusanos.size() == estado1.gusanos.size());
    std::vector<RepresentacionGusano> resultadoGusanos = resultado->gusanos[jugador];
    REQUIRE(resultadoGusanos.size() == listaGusanos.size());
    
    for (int i = 0; i < (int)resultadoGusanos.size(); i++) {
        RepresentacionGusano resultadoGusano = resultadoGusanos[i];
        RepresentacionGusano baseGusano = listaGusanos[i];
        RepresentacionArma resultadoArma = resultadoGusano.armaEquipada;
        RepresentacionArma baseArma = baseGusano.armaEquipada;
        REQUIRE(resultadoGusano.vida == baseGusano.vida);
        REQUIRE(resultadoGusano.idGusano == baseGusano.idGusano);
        REQUIRE(resultadoGusano.dir == baseGusano.dir);
        REQUIRE(resultadoGusano.posicion.first == baseGusano.posicion.first);
        REQUIRE(resultadoGusano.posicion.second == -baseGusano.posicion.second);
        REQUIRE(resultadoGusano.estado == baseGusano.estado);

        REQUIRE(resultadoArma.tieneMira ==baseArma.tieneMira);
        REQUIRE(resultadoArma.tienePotenciaVariable ==baseArma.tienePotenciaVariable);
        REQUIRE(resultadoArma.tieneCuentaRegresiva ==baseArma.tieneCuentaRegresiva);
        REQUIRE(resultadoArma.municiones ==baseArma.municiones);
        REQUIRE(resultadoArma.fragmentos ==baseArma.fragmentos);
        REQUIRE(resultadoArma.danio.epicentro ==baseArma.danio.epicentro);
        REQUIRE(resultadoArma.danio.radio ==baseArma.danio.radio);
        if (baseArma.fragmentos > 0) {
            REQUIRE(resultadoArma.danioFragmento.epicentro ==baseArma.danioFragmento.epicentro);
            REQUIRE(resultadoArma.danioFragmento.radio ==baseArma.danioFragmento.radio);
        }
        REQUIRE(resultadoArma.anguloRad ==baseArma.anguloRad);
        REQUIRE(resultadoArma.potencia ==baseArma.potencia);
        REQUIRE(resultadoArma.cuentaRegresiva ==baseArma.cuentaRegresiva);
        REQUIRE(resultadoArma.arma ==baseArma.arma);
    }  
}

// TEST 13

Accion obtenerAccionDeConfigurarAngulo(float angulo) {
    protocolo.configurarAngulo(angulo);
    return protocoloServer.obtenerAccion();
}

TEST_CASE( "Tests de configurar angulo", "[obtenerAccionDeConfigurarAngulo]" ) {
    Accion configurar;
    float angulo = 45.7;
    configurar.accion = PREPARAR;
    Configuracion config;
    config.caracteristica = ANGULO;
    config.angulo = angulo;
    configurar.configARealizar = config;

    Accion resultado = obtenerAccionDeConfigurarAngulo(angulo);
    REQUIRE(resultado.accion == configurar.accion);
    REQUIRE(resultado.configARealizar.caracteristica == config.caracteristica);
    REQUIRE(resultado.configARealizar.angulo == config.angulo);
}

// TEST 14

Accion obtenerAccionDeConfigurarPotencia(float potencia) {
    protocolo.configurarPotencia(potencia);
    return protocoloServer.obtenerAccion();
}

TEST_CASE( "Tests de configurar potencia", "[obtenerAccionDeConfigurarPotencia]" ) {
    Accion configurar;
    float potencia = 0.7;
    configurar.accion = PREPARAR;
    Configuracion config;
    config.caracteristica = POTENCIA;
    config.potencia = potencia;
    configurar.configARealizar = config;

    Accion resultado = obtenerAccionDeConfigurarPotencia(potencia);
    REQUIRE(resultado.accion == configurar.accion);
    REQUIRE(resultado.configARealizar.caracteristica == config.caracteristica);
    REQUIRE(resultado.configARealizar.potencia == config.potencia);
}

// TEST 15

Accion obtenerAccionDeConfigurarCuentaRegresiva(int cuentaRegresiva) {
    protocolo.configurarCuentaRegresiva(cuentaRegresiva);
    return protocoloServer.obtenerAccion();
}

TEST_CASE( "Tests de configurar cuenta regresiva", "[obtenerAccionDeConfigurarCuentaRegresiva]" ) {
    Accion configurar;
    int cuenta = 4;
    configurar.accion = PREPARAR;
    Configuracion config;
    config.caracteristica = CUENTA_REGRESIVA;
    config.cuentaRegresiva = cuenta;
    configurar.configARealizar = config;

    Accion resultado = obtenerAccionDeConfigurarCuentaRegresiva(cuenta);
    REQUIRE(resultado.accion == configurar.accion);
    REQUIRE(resultado.configARealizar.caracteristica == config.caracteristica);
    REQUIRE(resultado.configARealizar.cuentaRegresiva == config.cuentaRegresiva);
}