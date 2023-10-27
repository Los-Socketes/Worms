#ifndef GUSANO_HEADER
#define GUSANO_HEADER

#include "protocolo.h"
#include <sys/types.h>

// hp = healt points
typedef uint hp;

class Gusano {
private:
    hp vida;
    std::pair<int, int> coords;

public:
    Gusano(std::pair<int, int> coords);

    std::pair<int, int> cambio(Direccion accion);

    std::pair<int, int> getCoords();
    void setCoords(std::pair<int, int> nuevasCoords);
};



#endif
