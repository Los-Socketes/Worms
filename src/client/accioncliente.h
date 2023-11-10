#ifndef ACCIONCLIENTE_H_
#define ACCIONCLIENTE_H_

#include <utility>
#include "protocolo.h"

enum TipoComando{
    INVALIDO,
    SALIR,
    MOVER_CAMARA,
    TOGGLE_CAMARA,
    TAMANIO_VENTANA
};

struct Comando{
    TipoComando tipo;
    std::pair<int, int> parametros;

    Comando() : tipo(INVALIDO), parametros(0, 0) {}
    Comando(TipoComando tipo) : tipo(tipo), parametros(0, 0) {}
    Comando(TipoComando tipo, int p1, int p2) : tipo(tipo), parametros(p1,p2) {}
};

class AccionCliente{
    public:
        virtual void enviar(Protocolo& protocolo) = 0;
        virtual ~AccionCliente() = default;
};

class AccionMoverGusano : public AccionCliente {
    private:
        int id_gusano;
        Direccion direccion;
    public:
        AccionMoverGusano(int id_gusano, Direccion direccion);
        void enviar(Protocolo& protocolo) override;
        ~AccionMoverGusano() override = default;
};

class AccionDisparar : public AccionCliente {
    private:
        int id_gusano;
    public:
        AccionDisparar(int id_gusano);
        void enviar(Protocolo& protocolo) override;
        ~AccionDisparar() override = default;
};

class AccionEquiparArma : public AccionCliente {
    private:
        int id_gusano;
        ArmaProtocolo arma;
    public:
        AccionEquiparArma(int id_gusano, ArmaProtocolo arma);
        void enviar(Protocolo& protocolo) override;
        ~AccionEquiparArma() override = default;
};


#endif  // ACCIONCLIENTE_H_