#ifndef ESTADOJUEGO_H_
#define ESTADOJUEGO_H_

struct EstadoJuego {
    char coord_x;
    char coord_y;

    EstadoJuego(char coord_x, char coord_y) : coord_x(coord_x), coord_y(coord_y) {}
    
};

#endif  // ESTADOJUEGO_H_
