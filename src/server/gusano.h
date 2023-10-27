#ifndef GUSANO_HEADER
#define GUSANO_HEADER

#include "protocolo.h"
#include <sys/types.h>

// hp = healt points
typedef uint hp;

class Gusano {
private:
    hp vida;

public:
    Gusano();

    std::pair<int, int> cambio(Direccion accion);
};



#endif
