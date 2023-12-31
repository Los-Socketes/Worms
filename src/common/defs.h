#ifndef DEFS_HEADER
#define DEFS_HEADER

#include <string>
#include <vector>
#include <map>
#include <list>


//TODO: Anadir fucniones de conversion de coordenadas box2d a sdl

typedef int32_t id;
typedef int idJugador;
typedef int idProyectil;
typedef int idProvision;
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


#define VIENTOMAXIMODER 10
#define VIENTOMAXIMOIZQ -10

enum Direccion {INICIO_IZQ, FIN_IZQ, INICIO_DER, FIN_DER, SALTO, PIRUETA, INVAL_DIR};
enum EstadoGusano {QUIETO, CAMINANDO, SALTANDO, CAYENDO, DISPARANDO, HERIDO, HACE_PIRUETA, MUERTO, AHOGADO};
enum DireccionGusano {IZQUIERDA, DERECHA};
enum tipoInfo {PARTIDA, MAPA, INVAL_TIPO};
// Terminan con _P para diferenciarlas de las clases Arma y cada una en particular
// si los valores predeterminados cambiaran -> cambiar for en constructor de gusano
enum ArmaProtocolo {NADA_P, BAZOOKA_P, MORTERO_P, GRANADA_VERDE_P, GRANADA_ROJA_P, 
                    GRANADA_SANTA_P, BANANA_P, DINAMITA_P, BATE_P, ATAQUE_AEREO_P, 
                    TELETRANSPORTACION_P, INVAL_ARMA_P};
enum MomentoDePartida {ESPERANDO, POR_INICIAR, EN_MARCHA, TERMINADA, ESTADO_INVALIDO};
enum SituacionJugador {JUGANDO, GANASTE, PERDISTE};                    
/*
 *0. El gusano esta quieto
 *1. El gusano se mueve
 *2. El gusano equipa un arma
 *3. El gusano prepara/calibra el arma equipada
 *4. El gusano realiza el ataque con la calibracion previamente establecida
 */
//                    0         1         2        3          4
enum tipoAccion {ESTAQUIETO, MOVERSE, EQUIPARSE, PREPARAR, ATAQUE, ATACO, CHEAT, INVAL_ACCION};
enum ValorAConfigurar {ANGULO, POTENCIA, CUENTA_REGRESIVA, COORDENADAS};
enum TipoCheat {ARRANCAR_C, PROVISION_C, VIDA_C, DANIO_C, INVAL_CHEAT_C};

#define TIEMPOCAMBIOTURNO 60
#define TIEMPOCHANGUI 3

#define PARTIDAS 1
#define MAPAS 2
#define CREAR 3
#define UNIRSE 4
#define PEDIDO 5
#define EXITO 6
#define ERROR 7
#define EMPEZAR 13

#define ESTADO 8
// Codigos para acciones 
// mov + direccion -> izq, der, salto, pirueta
#define MOV 9
#define EQUIPAR 10
#define CALIBRAR 11
#define ATACAR 12
#define CHEATEAR 14


#define PIXELS_POR_METRO 20


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

typedef float radianes; 

struct RepresentacionDanio {
    int epicentro;
    int radio;
};

struct RepresentacionArma {
    bool tieneMira;
    bool tienePotenciaVariable;
    bool tieneCuentaRegresiva;

    int municiones;
    int fragmentos;

    RepresentacionDanio danio;
    RepresentacionDanio danioFragmento;

    // RADIANES ?!
    radianes anguloRad;
    float potencia;
    int cuentaRegresiva;
    ArmaProtocolo arma;
};

struct RepresentacionGusano {
    id idGusano;
    hp vida;
    DireccionGusano dir;
    EstadoGusano estado;
    std::pair<coordX, coordY> posicion;
    RepresentacionArma armaEquipada;
};

enum tipoProvision { MUNICION, VIDA };

struct RepresentacionProvisiones {
    idProvision id;
    std::pair<coordX, coordY> posicion;
    bool estaEnElAire;
    tipoProvision tipo;
    ArmaProtocolo armaMunicion;
    bool fueAgarrada;
    bool exploto;
};

struct RepresentacionViga {
    radianes angulo;
    //WARNING: En teoria esto solo pueden valer 6 o 3. Mepa que
    //es mas facil hacer una longitud generia y pasar el int
    int longitud;
    //ATTENTION: Esta es la posicion INICIAL,  
    std::pair<coordX, coordY> posicionInicial;
};


struct InformacionInicial {
    idJugador jugador;
    std::vector<RepresentacionViga> vigas;
    std::pair<coordX, coordY> dimensiones;
    id idPartida;

};

struct RepresentacionProyectil {
    idProyectil id;
    ArmaProtocolo proyectil;
    bool esFragmento;
    std::pair<coordX,coordY> posicion;
    radianes angulo;
    int cuentaRegresiva;
    bool exploto;
};


// Tiene la info del estado del juego
struct EstadoDelJuego {
    std::map<idJugador, std::map<id, RepresentacionGusano>> gusanos;
    std::vector<RepresentacionProyectil> proyectiles;
    idJugador jugadorDeTurno;
    id gusanoDeTurno;
    int segundosRestantes;
    MomentoDePartida momento;
    std::map<idJugador, SituacionJugador> situacionJugadores;    
    std::vector<RepresentacionProvisiones> provisiones;
    int viento;
    int ronda;
};

struct Configuracion {
    ValorAConfigurar caracteristica;
    // WARNING: saco el union porque rompia con el std::pair
    float potencia;
    float angulo;
    int cuentaRegresiva;
    std::pair<coordX, coordY> coordenadas;
};

struct Accion {
    id idGusano;
    idJugador jugador;
    tipoAccion accion;
    // WARNING: saco el union porque rompia con el std::pair
    // :(
    Direccion dir;
    ArmaProtocolo armaAEquipar;
    Configuracion configARealizar;
    TipoCheat cheat;
    bool esEmpezar;

};

#endif
