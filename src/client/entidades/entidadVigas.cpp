#include "entidadVigas.h"

EntidadVigas::EntidadVigas(GestorAnimaciones& gestor_animaciones,
    Camara& camara) :
    Entidad(gestor_animaciones),
    camara(camara),
    vigas() {}

void EntidadVigas::setVigas(std::vector<RepresentacionViga> vigas) {
    this->vigas = vigas;
}

void EntidadVigas::dibujar() {
    std::pair<int, int> posicion;
    radianes ang;
    std::shared_ptr<Animacion> imagen_viga;
    // Dibujo las vigas.
    for (auto& viga : vigas) {
        // Traduzco las coordenadas de la viga.
        posicion = camara.traducirCoordenadas(viga.posicionInicial.first, viga.posicionInicial.second);
        ang = -viga.angulo;
        if (viga.longitud > 3) {
            imagen_viga = gestor_animaciones.getAnimacionEscenario(VIGA_GRANDE);
        } else {
            imagen_viga = gestor_animaciones.getAnimacionEscenario(VIGA_CHICA);
        }  
        imagen_viga->setDimensiones(viga.longitud * PIXELS_POR_METRO, 0.8 * PIXELS_POR_METRO);
        imagen_viga->dibujar(camara, posicion.first, posicion.second, false, it, 1, ang);
    }    
}