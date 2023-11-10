#ifndef BOX2DDEFS_HEADER
#define BOX2DDEFS_HEADER

    /*
     *Coordenadas en Box2d:
     *Arranca abajo a la izquierda.
     *X:
     *	>: +1
     *	<: -1
     *Y:
     *	^: +1
     *	v: -1
    */

// Constantes de gravedad.
// WARNING: Originalmente valia 10 no 100. Ver que onda.
#define fuerzaGravitariaX 0.0f
#define fuerzaGravitariaY -10.0f

#define anchoViga 0.8f
#define longitudVigaGrande 6.0f
#define longitudVigaChica 3.0f

#define VELOCIDADMOVIMIENTO 10.0f

#define masaCuerpoEstatico 0.0f

#endif
