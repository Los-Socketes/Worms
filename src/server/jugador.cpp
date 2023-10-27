#include "jugador.h"
#include "protocolo.h"

Jugador::Jugador(Protocolo &&):
    protocolo(std::move(protocolo)){
}
