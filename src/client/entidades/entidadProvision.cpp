#include "entidadProvision.h"

EntidadProvision::EntidadProvision(GestorAnimaciones& gestor_animaciones,
    Renderer& renderizador,
    std::shared_ptr<EstadoDelJuego>& estado_juego,
    idProvision id_provision,
    GestorSonidos& gestor_sonidos,
    Camara& camara,
    Font& fuente1,
    Font& fuente2) :
    EntidadActualizable(estado_juego, renderizador, gestor_animaciones, camara, fuente1, fuente2),
    id_provision(id_provision),
    gestor_sonidos(gestor_sonidos),
    agarrada(false) {}

const std::string EntidadProvision::nombreArma(ArmaProtocolo& armaMunicion) {
    std::string nombre_arma = "";
    switch (armaMunicion) {
        case BAZOOKA_P:
            nombre_arma = "Bazooka";
            break;
        case MORTERO_P:
            nombre_arma = "Mortero";
            break;
        case GRANADA_VERDE_P:
            nombre_arma = "Granada verde";
            break;
        case GRANADA_ROJA_P:
            nombre_arma = "Granada roja";
            break;
        case GRANADA_SANTA_P:
            nombre_arma = "Granada santa";
            break;
        case BANANA_P:
            nombre_arma = "Banana";
            break;
        case DINAMITA_P:
            nombre_arma = "Dinamita";
            break;
        case ATAQUE_AEREO_P:
            nombre_arma = "Ataque aereo";
            break;
        case TELETRANSPORTACION_P:
            nombre_arma = "Teletransportacion";
            break;
        case BATE_P:
            nombre_arma = "Bate";
            break;
        case NADA_P:
            nombre_arma = "";
            break;
        default:
            nombre_arma = "";
            break;
    }
    return nombre_arma;
}

void EntidadProvision::dibujarNombreArma(ArmaProtocolo& armaMunicion, int& pos_x, int& pos_y) {
    // Acomodo la posicion para que quede centrada en la provision.
    int posicion_x = pos_x - 15;
    int posicion_y = pos_y - 30;

    std::optional<Rect> rect_interseccion = camara.getRectangulo().GetIntersection(Rect(posicion_x, posicion_y, 30, 15));
    
    int coord_x = posicion_x - camara.getPosicionX();
    int coord_y = posicion_y - camara.getPosicionY();
    
    // Si no hay interseccion no se renderiza.
    if (!rect_interseccion) {
        return;
    }

    // Obtengo el nombre del arma.
    std::string nombre_arma = nombreArma(armaMunicion);
    // Dibujo el nombre de la provision.
    fuente1.SetOutline(0);
    Texture textura_vida(renderizador, fuente1.RenderText_Blended(nombre_arma, {255, 255, 255, 255}));
    renderizador.Copy(textura_vida, NullOpt, Rect(coord_x, coord_y, 30, 15));    
}

void EntidadProvision::dibujarProvision(tipoProvision& tipo, bool& estaEnElAire, ArmaProtocolo& armaMunicion, int& pos_x, int& pos_y) {
    ItemEscenario caja;
    if (tipo == VIDA) {
        if (estaEnElAire)
            caja = PROVISION_MEDICINA_CAYENDO;
        else
            caja = PROVISION_MEDICINA;
    } else {
        if (estaEnElAire)
            caja = PROVISION_MUNICION_CAYENDO;
        else
            caja = PROVISION_MUNICION;
    }
    std::shared_ptr<Animacion> animacion = gestor_animaciones.getAnimacionEscenario(caja);
    actualizarAnimacion(animacion);
    animacion->dibujar(camara, pos_x, pos_y, false, it, 1);
    if (tipo == MUNICION) {
        dibujarNombreArma(armaMunicion, pos_x, pos_y);
    }
    if (it == 0 && !estaEnElAire)
        gestor_sonidos.getSonido(SONIDO_IMPACTO_CAJA)->reproducir();
}

void EntidadProvision::dibujar() {
    RepresentacionProvisiones provision;
    // Si la provision no existe, no se dibuja y se pone como muerto.
    bool existe = false;
    for (auto& provision_estado : estado_juego->provisiones) {
        if (provision_estado.id == id_provision) {
            provision = provision_estado;
            existe = true;
            break;
        }
    }
    if (!existe) {
        muerta = true;
        return;
    }
    // Traduzco la posicion de la provision.
    std::pair<int, int> pos = camara.traducirCoordenadas(provision.posicion.first, provision.posicion.second);
    // Dibujo la provision.
    dibujarProvision(provision.tipo, provision.estaEnElAire, provision.armaMunicion, pos.first, pos.second);
    if (agarrada == false && provision.fueAgarrada == true) {
        agarrada = true;
        gestor_sonidos.getSonido(SONIDO_AGARRANDO_CAJA)->reproducir();
    }
}
