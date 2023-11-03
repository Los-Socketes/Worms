#ifndef GUSANO_HEADER
#define GUSANO_HEADER

#include "protocolo.h"
#include <sys/types.h>
#include "defs.h"

// hp = healt points
typedef uint hp;

class Gusano {
private:
    hp vida;
    DireccionGusano direccion;
    std::pair<int, int> coords;

    //Representa que el gusano se esta moviendo aka esta en un estado
    //de moverser
    // bool moviendose = false;

public:
    Gusano(std::pair<int, int> coords);

    std::pair<int, int> cambio(Accion accion);
    [[nodiscard]] DireccionGusano getDireccion();

    std::pair<int, int> getCoords();
    void setCoords(std::pair<int, int> nuevasCoords);


    void detener();
    void ponerEnMovimiento();
};



#endif
