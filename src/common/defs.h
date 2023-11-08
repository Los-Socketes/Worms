#ifndef DEFS_HEADER
#define DEFS_HEADER

#include <string>
#include <vector>
#include <map>
#include <list>


//TODO: Anadir fucniones de conversion de coordenadas box2d a sdl

typedef int32_t id;
typedef int idJugador;
typedef uint hp;

#define strings std::vector<std::string>

// Estos dos typedefs representan las COORDENADAS de un objeto en la
// partida
typedef float coordX;
typedef float coordY;

// Estos dos typedefs representan el CAMBIO deseado por un objeto
typedef float cambioX;
typedef float cambioY;

#define INVAL_ID (id)-1
#define noIgn [[nodiscard]]

//INICIO_IZQ, FIN_IZQ, INICIO_DER, FIN_DER, SALTO, PIRUETA, INVAL_DIR
enum Direccion {INICIO_IZQ, FIN_IZQ, INICIO_DER, FIN_DER, SALTO, PIRUETA, INVAL_DIR};
// enum class AccionServer { MOV_IZQ, MOV_DER, MOV_SALTO, MOV_PIRUETA,
// MOV_QUIETO };
enum EstadoGusano {QUIETO, CAMINANDO, SALTANDO, CAYENDO};
enum DireccionGusano {IZQUIERDA, DERECHA};
enum tipoInfo {PARTIDA, MAPA, INVAL_TIPO};

/*
 *0. El gusano esta quieto
 *1. El gusano se mueve
 *2. El gusano equipa un arma
 *3. El gusano prepara/calibra el arma equipada
 *4. El gusano realiza el ataque con la calibracion previamente establecida
 */
//                  0        1          2        3
enum tipoAccion {ESTAQUIETO, MOVERSE, EQUIPARSE, PREPARAR, ATAQUE};

#define PARTIDAS 1
#define MAPAS 2
#define CREAR 3
#define UNIRSE 4
#define PEDIDO 5
#define EXITO 6
#define ERROR 7

#define ESTADO 8
// Codigos para acciones 
// mov + direccion -> izq, der, salto, pirueta
#define MOV 9
// este se usa para ataques cuerpo a cuerpo
#define ATACAR 10
#define DISPARAR 11

// Tiene la info de una partida para unirse
struct RepresentacionPartida {
    id ID;
};

// Tiene la info de un mapa
struct RepresentacionMapa {
    id ID;
    std::string nombre;
};

// Estas macros permiten que hagas:
// posicion.enX
// posicion.enY
#define enX first
#define enY second

struct RepresentacionGusano {
    id idGusano;
    hp vida;
    DireccionGusano dir;
    // TODO: Agregar recibir y enviar estado
    EstadoGusano estado;
    std::pair<coordX, coordY> posicion;
    // TODO: agregar arma en mano
};


// Tiene la info del estado del juego
struct EstadoDelJuego {
    std::map<idJugador, std::vector<RepresentacionGusano>> gusanos;
    // DireccionGusano dir;
    // std::pair<coordX, coordY> posicion;
};

struct Accion {
    id idGusano;
    idJugador jugador;
    tipoAccion accion;
    // union Argumento{
        Direccion dir;
    // };
};

#endif
