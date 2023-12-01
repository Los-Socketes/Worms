#include "entidadActualizable.h"

EntidadActualizable::EntidadActualizable(std::shared_ptr<EstadoDelJuego>& estado_juego, 
    Renderer& renderizador, GestorAnimaciones& gestor_animaciones, Camara& camara,
    Font& fuente1, Font& fuente2) : 
    Entidad(gestor_animaciones),
    estado_juego(estado_juego),
    muerta(false),
    renderizador(renderizador),
    camara(camara),
    fuente1(fuente1),
    fuente2(fuente2),
    animacion_actual(gestor_animaciones.getImagenIcono(NADA_P)) {}

void EntidadActualizable::actualizarAnimacion(std::shared_ptr<Animacion> animacion) {
    // Si la animacion actual es distinta a la nueva, la actualizo y reseteo el contador.
    if (animacion_actual != animacion) {
        animacion_actual = animacion;
        it = 0;
    }
}

void EntidadActualizable::dibujarCuentaRegresiva(std::pair<int,int>& posicion, float& cuenta_regresiva) {
    // Dibujo el temporizador al lado del gusano o proyectil.
    int pos_x = posicion.first + 20;
    int pos_y = posicion.second - 30;

    std::optional<Rect> rect_interseccion = camara.getRectangulo().GetIntersection(Rect(pos_x, pos_y, 16, 16));

    int coord_x = pos_x - camara.getPosicionX();
    int coord_y = pos_y - camara.getPosicionY();

    // Si no hay interseccion no se renderiza.
    if (!rect_interseccion) {
        return;
    }
    
    // Me quedo sólo con los 3 primeros digitos.
    std::string cuenta_regresiva_str = std::to_string(cuenta_regresiva);
    if (cuenta_regresiva_str.size() > 4) {
        cuenta_regresiva_str = cuenta_regresiva_str.substr(0, 4);
    }

    // Dibujo el borde del temporizador.
    fuente1.SetOutline(2);
    Texture textura_cuenta_outline(renderizador, fuente1.RenderText_Blended(cuenta_regresiva_str, {0, 0, 0, 255}));
    renderizador.Copy(textura_cuenta_outline, NullOpt, Rect(coord_x, coord_y, 16, 14));

    // Dibujo el temporizador.
    fuente1.SetOutline(0);
    Texture textura_cuenta(renderizador, fuente1.RenderText_Blended(cuenta_regresiva_str, {255, 255, 255, 255}));
    renderizador.Copy(textura_cuenta, NullOpt, Rect(coord_x, coord_y, 16, 14));
}

bool EntidadActualizable::estaMuerta() {
    return muerta;
}
