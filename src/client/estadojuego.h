#ifndef ESTADOJUEGO_H_
#define ESTADOJUEGO_H_

struct EstadoJuego {
    int32_t id;
    int coord_x;
    int coord_y;
    int dir_x;

    EstadoJuego(int coord_x, int coord_y, int dir_x) : 
        coord_x(coord_x), coord_y(coord_y), dir_x(dir_x) {
        id = 0;
    }
    
};

#endif  // ESTADOJUEGO_H_
