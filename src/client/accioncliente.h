#ifndef ACCIONCLIENTE_H_
#define ACCIONCLIENTE_H_

#include <utility>
#include "protocolo.h"

enum TipoComando{
    INVALIDO,
    SALIR,
    MOVER_CAMARA,
    TOGGLE_CAMARA,
    TAMANIO_VENTANA,
    MOVER_CURSOR,
    VOLUMEN_MAS,
    VOLUMEN_MENOS,
    TOGGLE_MUTEAR,
    SIN_MUNICIONES
};

// Comandos internos del cliente.
struct Comando{
    TipoComando tipo;
    std::pair<int, int> parametros;

    Comando() : tipo(INVALIDO), parametros(0, 0) {}
    Comando(TipoComando tipo) : tipo(tipo), parametros(0, 0) {}
    Comando(TipoComando tipo, int p1, int p2) : tipo(tipo), parametros(p1,p2) {}
};

// Clase abstracta que representa una accion que el cliente puede realizar.
class AccionCliente {
 public:
    virtual void enviar(Protocolo& protocolo) = 0;
    virtual ~AccionCliente() = default;
};

// Accion de movimiento. Se envia la direccion a la que se mueve.
class AccionMoverGusano : public AccionCliente {
 private:
    Direccion direccion;
 public:
    AccionMoverGusano(Direccion direccion);
    void enviar(Protocolo& protocolo) override;
    ~AccionMoverGusano() override = default;
};

// Accion de cambio de angulo. Se envia el cambio a realizar.
class AccionCambiarAngulo : public AccionCliente {
 private:
    radianes angulo;
 public:
    AccionCambiarAngulo(radianes angulo);
    void enviar(Protocolo& protocolo) override;
    ~AccionCambiarAngulo() override = default;
};

// Accion de cambio de coordenadas. Se envian las nuevas coordenadas.
class AccionCambiarCoordenadas : public AccionCliente {
 private:
    coordX x;
    coordY y;
 public:
    AccionCambiarCoordenadas(coordX x, coordY y);
    void enviar(Protocolo& protocolo) override;
    ~AccionCambiarCoordenadas() override = default;
};

// Accion de cambio de potencia. Se envia el cambio a realizar.
class AccionCambiarPotencia : public AccionCliente {
 private:
    float potencia;
 public:
    AccionCambiarPotencia(float potencia);
    void enviar(Protocolo& protocolo) override;
    ~AccionCambiarPotencia() override = default;
};

// Accion de cambio de cuenta regresiva. Se envia la nueva cuenta regresiva.
class AccionCambiarCuentaRegresiva : public AccionCliente {
 private:
    int cuenta_regresiva;
 public:
    AccionCambiarCuentaRegresiva(int cuenta_regresiva);
    void enviar(Protocolo& protocolo) override;
    ~AccionCambiarCuentaRegresiva() override = default;
};

// Accion de disparo. Solamente se indica que se dispara el arma equipada.
class AccionDisparar : public AccionCliente {
 public:
    AccionDisparar();
    void enviar(Protocolo& protocolo) override;
    ~AccionDisparar() override = default;
};

// Accion de cambio de arma. Se envia el arma a equipar.
class AccionEquiparArma : public AccionCliente {
 private:
    ArmaProtocolo arma;
 public:
    AccionEquiparArma(ArmaProtocolo arma);
    void enviar(Protocolo& protocolo) override;
    ~AccionEquiparArma() override = default;
};


#endif  // ACCIONCLIENTE_H_