#ifndef DEFS_HEADER
#define DEFS_HEADER

#include <string>
#include <vector>
#include <map>
#include <list>



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
enum class AccionServer { MOV_IZQ, MOV_DER, MOV_SALTO, MOV_PIRUETA, MOV_QUIETO };
enum DireccionGusano {IZQUIERDA, DERECHA};
enum tipoInfo {PARTIDA, MAPA, INVAL_TIPO};

enum tipoAccion {MOVERSE, ATAQUE_CUERPO};

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
#define ATAQUE 10
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
    tipoAccion accion;
    Direccion dir;
};

#endif
