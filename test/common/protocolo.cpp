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

std::pair<InformacionInicial,id> crearPartidaCasoFeliz(id mapa, InformacionInicial partida) {
    protocoloServer.enviarConfirmacion(partida);
    InformacionInicial partidaRecibida = protocolo.crearPartida(mapa);
    id idMapa = protocoloServer.obtenerMapaDeseado();
    std::pair<InformacionInicial,id> ids(partidaRecibida, idMapa);
    return ids;
}

TEST_CASE("Test de crear partidas (Caso feliz)", "[crearPartidaCasoFeliz]") {
    InformacionInicial info;
    info.jugador = 1;

    std::pair<coordX, coordY> dimensiones(60.5, 85.42);
    info.dimensiones = dimensiones;

    RepresentacionViga viga1;
    viga1.angulo = 45.2;
    viga1.longitud = 3;
    viga1.posicionInicial.enX = 2.3;
    viga1.posicionInicial.enY = 4.2;

    RepresentacionViga viga2;
    viga2.angulo = 38.3;
    viga2.longitud = 6;
    viga2.posicionInicial.enX = 20.6;
    viga2.posicionInicial.enY = 5.4;

    std::vector<RepresentacionViga> vigas = {viga1, viga2};
    info.vigas = vigas;
    std::pair<InformacionInicial,id> resultado = crearPartidaCasoFeliz((id)0, info);
    for (int i = 0; i < (int)vigas.size(); i++) {
        RepresentacionViga vigaObtenida = resultado.first.vigas[i];
        RepresentacionViga vigaBase = vigas[i];
        REQUIRE(vigaObtenida.angulo == vigaBase.angulo);
        REQUIRE(vigaObtenida.longitud == vigaBase.longitud);
        REQUIRE(vigaObtenida.posicionInicial.enX == vigaBase.posicionInicial.enX);
        REQUIRE(vigaObtenida.posicionInicial.enY == vigaBase.posicionInicial.enY);
    }
    REQUIRE(resultado.first.jugador == 1);
    REQUIRE(resultado.first.dimensiones.enX == dimensiones.enX);
    REQUIRE(resultado.first.dimensiones.enY == dimensiones.enY);
    REQUIRE(resultado.second == (id)0);

}

// TEST 7

std::pair<InformacionInicial,id> crearPartidaCasoError(id mapa) {
    protocoloServer.enviarError();
    InformacionInicial partida = protocolo.crearPartida(mapa);
    id idMapa = protocoloServer.obtenerMapaDeseado();
    std::pair<InformacionInicial,id> ids(partida, idMapa);
    return ids;
}

TEST_CASE("Test de crear partidas (Caso error)", "[crearPartidaCasoError]") {
    std::pair<InformacionInicial,id> resultado = crearPartidaCasoError((id)0);
    REQUIRE(resultado.first.jugador == INVAL_ID);
    REQUIRE(resultado.second == (id)0);
}

// TEST 8

std::pair<InformacionInicial,id> unirsePartidaCasoFeliz(id partida, InformacionInicial infoPartida) {
    protocoloServer.enviarConfirmacion(infoPartida);
    InformacionInicial res = protocolo.unirseAPartida(partida);
    id idPartida = protocoloServer.obtenerPartidaDeseada();
    std::pair<InformacionInicial,id> resultado(res, idPartida);
    return resultado;
}

TEST_CASE("Test de unirse a partidas (Caso feliz)", "[unirsePartidaCasoFeliz]") {
    InformacionInicial info;
    info.jugador = 1;

    std::pair<coordX, coordY> dimensiones(87.45, 50.21);
    info.dimensiones = dimensiones;

    RepresentacionViga viga1;
    viga1.angulo = 45;
    viga1.longitud = 3;
    viga1.posicionInicial.enX = 2.3;
    viga1.posicionInicial.enY = 4.2;

    RepresentacionViga viga2;
    viga2.angulo = 38.5;
    viga2.longitud = 6;
    viga2.posicionInicial.enX = 20.6;
    viga2.posicionInicial.enY = 5.4;

    std::vector<RepresentacionViga> vigas = {viga1, viga2};
    info.vigas = vigas;
    std::pair<InformacionInicial,id> resultado = unirsePartidaCasoFeliz((id)0, info);
    REQUIRE(resultado.first.jugador == 1);
    REQUIRE(resultado.first.dimensiones.enX == dimensiones.enX);
    REQUIRE(resultado.first.dimensiones.enY == dimensiones.enY);
    REQUIRE(resultado.first.vigas.size() == vigas.size());
    for (int i = 0; i < (int)vigas.size(); i++) {
        RepresentacionViga vigaObtenida = resultado.first.vigas[i];
        RepresentacionViga vigaBase = vigas[i];
        REQUIRE(vigaObtenida.angulo == vigaBase.angulo);
        REQUIRE(vigaObtenida.longitud == vigaBase.longitud);
        REQUIRE(vigaObtenida.posicionInicial.enX == vigaBase.posicionInicial.enX);
        REQUIRE(vigaObtenida.posicionInicial.enY == vigaBase.posicionInicial.enY);
    }
    REQUIRE(resultado.second == (id)0); 
}

// TEST 9

std::pair<InformacionInicial,id> unirsePartidaCasoError(id partida) {
    protocoloServer.enviarError();
    InformacionInicial res = protocolo.unirseAPartida(partida);
    id idPartida = protocoloServer.obtenerPartidaDeseada();
    std::pair<InformacionInicial,id> resultado(res, idPartida);
    return resultado;
}

TEST_CASE("Test de unirse a partidas (Caso error)", "[unirsePartidaCasoError]") {
    std::pair<InformacionInicial,id> resultado = unirsePartidaCasoError((id)0);
    REQUIRE(resultado.first.jugador == INVAL_ID);
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
    protocoloServer.enviarEstadoDelJuego(estado);
    return protocolo.obtenerEstadoDelJuego();
}

TEST_CASE( "Tests de enviar estado del Juego", "[enviarEstadoDelJuego]" ) {
    idJugador jugador = 0;
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

    RepresentacionProyectil proyectil1;
    proyectil1.id = 8;
    proyectil1.proyectil = DINAMITA_P;
    proyectil1.esFragmento = false;
    proyectil1.posicion.enX = 20.6;
    proyectil1.posicion.enY = 4.98;
    proyectil1.angulo = 49.1;
    proyectil1.cuentaRegresiva = 0;
    proyectil1.exploto = true;

    RepresentacionProyectil proyectil2;
    proyectil1.id = 9;
    proyectil2.proyectil = GRANADA_ROJA_P;
    proyectil2.esFragmento = true;
    proyectil2.posicion.enX = 4.6;
    proyectil2.posicion.enY = 7.8;
    proyectil2.angulo = 8.6;
    proyectil2.cuentaRegresiva = 0;
    proyectil2.exploto = false;

    std::vector<RepresentacionProyectil> proyectiles = {proyectil1, proyectil2};

    std::shared_ptr<EstadoDelJuego> estado(new EstadoDelJuego);
    estado->gusanos = gusanos;
    estado->proyectiles = proyectiles;
    estado->jugadorDeTurno = jugador;
    estado->gusanoDeTurno = (id)2;
    
    std::shared_ptr<EstadoDelJuego> resultado = enviarEstadoDelJuego(estado);
    REQUIRE(resultado->gusanos.size() == gusanos.size());
    REQUIRE(resultado->jugadorDeTurno == estado->jugadorDeTurno);
    REQUIRE(resultado->gusanoDeTurno == estado->gusanoDeTurno);
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
        REQUIRE(resultadoGusano.posicion.second == baseGusano.posicion.second);
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

    std::vector<RepresentacionProyectil> resultadoProyectiles = resultado->proyectiles;
    REQUIRE(resultadoProyectiles.size() == proyectiles.size());
    for (int i = 0; i < (int)proyectiles.size(); i++) {
        RepresentacionProyectil resultadoProyectil = resultadoProyectiles[i];
        RepresentacionProyectil baseProyectil = proyectiles[i];

        REQUIRE(resultadoProyectil.proyectil == baseProyectil.proyectil);
        REQUIRE(resultadoProyectil.esFragmento == baseProyectil.esFragmento);
        REQUIRE(resultadoProyectil.posicion.enX == baseProyectil.posicion.enX);
        REQUIRE(resultadoProyectil.posicion.enY == baseProyectil.posicion.enY);
        REQUIRE(resultadoProyectil.angulo == baseProyectil.angulo);
        REQUIRE(resultadoProyectil.cuentaRegresiva == baseProyectil.cuentaRegresiva);
        REQUIRE(resultadoProyectil.exploto == baseProyectil.exploto);
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

// TEST 16

Accion obtenerAccionDeConfigurarCoordenadas(std::pair<coordX, coordY> coordenadas) {
    protocolo.configurarCoordenadas(coordenadas);
    return protocoloServer.obtenerAccion();
}

TEST_CASE( "Tests de configurar coordenadas", "[obtenerAccionDeConfigurarCoordenadas]" ) {
    Accion configurar;
    std::pair<coordX,coordY> coordenadas(1.5,4.6);
    configurar.accion = PREPARAR;
    Configuracion config;
    config.caracteristica = COORDENADAS;
    config.coordenadas = coordenadas;
    configurar.configARealizar = config;

    Accion resultado = obtenerAccionDeConfigurarCoordenadas(coordenadas);
    REQUIRE(resultado.accion == configurar.accion);
    REQUIRE(resultado.configARealizar.caracteristica == config.caracteristica);
    REQUIRE(resultado.configARealizar.coordenadas.enX == config.coordenadas.enX);
    REQUIRE(resultado.configARealizar.coordenadas.enY == config.coordenadas.enY);
}