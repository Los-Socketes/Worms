#ifndef ESTADOJUEGO_H_
#define ESTADOJUEGO_H_

struct EstadoJuego {
    int32_t id;
    int coord_x;
    int coord_y;

    EstadoJuego(int coord_x, int coord_y) : coord_x(coord_x), coord_y(coord_y) {
        id = 0;
    }
    
};

#endif  // ESTADOJUEGO_H_
