#ifndef DEFS_HEADER
#define DEFS_HEADER

#include <string>
#include <vector>


typedef int32_t id;
typedef int idJugador;

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
enum class Accion { MOV_IZQ, MOV_DER, MOV_SALTO, MOV_PIRUETA, MOV_QUIETO };
enum DireccionGusano {IZQUIERDA, DERECHA};
enum tipoInfo {PARTIDA, MAPA, INVAL_TIPO};

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

// Tiene la info del estado del juego
// Por ahora solo un gusano con posicion y direccion
struct EstadoDelJuego {
    DireccionGusano dir;
    std::pair<coordX, coordY> posicion;
};

#endif
