#ifndef DEFS_HEADER
#define DEFS_HEADER

#include <string>


typedef int32_t id;
#define INVAL_ID (id)-1
enum Direccion {INICIO_IZQ, FIN_IZQ, INICIO_DER, FIN_DER, SALTO, PIRUETA, INVAL_DIR};
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
#define noIgn [[nodiscard]]

struct RepresentacionPartida {
    id ID;
};

struct RepresentacionMapa {
    id ID;
    std::string nombre;
};

#endif
