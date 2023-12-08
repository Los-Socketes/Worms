# Manual del proyecto
## División del trabajo
El proyecto originalmente se dividió en:
- Cliente (realizado por Juan Pablo Fresia)
- Server (realizado por Tomás Fabrizio Orsi)
- Protocolo + concurrencia (realizado por Antonella Jazmín Pellegrini)

A continuación se presenta una lista más detallada de que implementó cada integrante
### Juan Pablo Fresia
- Lógica del cliente
- Gráficos con SDL
- Sonidos con SDL
- Manual de usuario

### Tomás Fabrizio Orsi
- Lógica del server
- Box2d
- CMake
- Configuración por yaml de server
- Github Actions

### Antonella Jazmín Pellegrini
- Protocolo (código y tests)
- Valgrind del server
- Configuración por yaml de armas, mapas y cliente
- Menú con QT
- Daño por caída
- Generalización de recibir daño + salto post recibir daño
- Parábola de proyectil + velocidad en base a potencia + representación del ángulo
- Cambio de elasticidad para que el arma "banana" rebote más que el resto de armas

## División semanal
La división semanal esta presentada en base a las reuniones que el grupo tuvo con las correctoras.
### Semana 1 (20 de Octubre a 27 de Octubre)
Se pidió tener una conexión funcional entre server y cliente, se podía demostrar con el movimiento del gusano.
Se presentó un esqueleto con conversación inicial entre server y cliente para crear una partida. Además, del lado del cliente, se presentó que SDL mostraba una pantalla, renderizaba y se registraba el uso de las flechas.

### Semana 2 (27 de Octubre a 3 de Noviembre)
Se pidió tener un gusano moviéndose y configuración con archivos yaml. Además, se pidió reestructurar el proyecto para que cliente y jugador sean una sola clase.
Se presentó un gusano con animación de movimiento (ya con movimiento enviado por socket) y el refactor pedido.

### Semana 3 (3 de Noviembre a 10 de Noviembre)
Se pidió tener implementada alguna arma para verificar lógica de bajar vida, incluir box2d, tener la concurrencia resuelta y reestructurar el proyecto para que el comportamiento de jugador vuelva a estar dividido en 2 clases (cliente y jugador).
Se presentó un juego en el que se podían conectar mas de 1 gusano a la misma partida. Se implementó gravedad, movimiento y colisión con box2d. Se presentó el mapa renderizado en capas con fondo, agua y gusanos, además de movimiento de cámara y animaciones para equipar bate y atacar. De parte del protocolo se presentaron tests para todos los métodos públicos además de cambios para permitir los avances de cliente y servidor. Por último, se presentó una base de cómo se representarán las armas y el archivo de configuración de las mismas.

### Semana 4 (10 de Noviembre a 17 de Noviembre)
Se pidió tener implementadas 3 armas mas, además de poder tenerlas en un menú. Además del lado del cliente se pidió agregar el manejador de texturas. Para el server se pidió tener un juego "funcional" de forma que interactúen los gusanos y se puedan atacar los unos a los otros.
Se presentó un menú con todas las armas que se pueden equipar. Se conectan varios gusanos, se golpean entre ellos y se baja la vida. Para el bate se puede setear el ángulo y tiene los límites correctos. Se presentó una base de teletransportación que funciona pero envía una posición constante. De parte del protocolo se modificó el envío y obtención del estado del juego para que sea con shared pointers, también se agregaron los mensajes para la configuración de ataque (ángulo, potencia, cuenta regresiva, coordenadas teletransportación).

### Semana 5 (17 de Noviembre a 24 de Noviembre)
Se pidió tener implementadas más armas con hitboxes (u otra forma, si se puede hacer). Modificar cómo se aplica el daño para que la entidad sepa con que esta colisionando y cómo afecta la otra entidad. Se pide agregar readme con instrucciones de ejecución para la entrega parcial. También se pide tener el menú con QT implementado y poder eliminar los elementos de box2d. 
Se presentaron 3 armas funcionales, bate de baseball, dinamita y teletransporte, se gráfica también cuando se cambia el temporizador y la potencia. Ademas se presentó el menú hecho en QT. Al juego se le agregaron los sonidos y música ambiente. Se arreglaron los errores de valgrind del lado server para que no pierda memoria. Se programaron también para que se vean las animaciones de todos los proyectiles y fragmentos y se actualizó el estado del juego para que envíe los proyectiles a graficar. Se implementó también una clase para configurar los datos que usa el cliente en base a un yaml. Ademas se agregó la lógica de turnos y tiempo restante. Se consiguió eliminar los cuerpos de box2d.

### Semana 6 (24 de Noviembre a 1 de Diciembre)
Se dieron correcciones en un Excel por la primera entrega del trabajo, se clasificaban desde P1 (las mas importantes) a P5 (las menos importantes). Se pidió tener implementada la bazooka obligatoriamente e intentar tener mas armas.
Se presentaron todas las armas funcionales, se arreglaron para que las condiciones de cambio de turno funcionen correctamente, se agregaron mas datos al estado del juego para poder mostrar una pantalla final en base a si el jugador gano o perdió. Se arregló el salto para que funcione mejor y se pueda mover entre vigas. Se crearon más mapas y también se implementó el daño por caída. Se generalizó la función de recibir daño para que sea en base al arma que realiza el daño. Además se arreglaron bugs y se agregó un filtro al enviar las partidas disponibles para que solo se puedan unir a partidas que todavía no inician. Se agregaron los mapas al yaml y se implementó la lógica para asignar gusanos y sumarles vida en caso de que un jugador tenga menos gusanos. También se arregló el movimiento de la cámara para que siga la consigna.

### Semana 7 (1 de Diciembre a 5 de Diciembre)
Se pidió implementar las provisiones y viento. Se sugirieron un par de cheats que consideraban útiles como el de iniciar la partida con un solo jugador (una cantidad menor a la mínima que se deja).
Se hizo la entrega final con provisiones de vida, municiones y trampa donde dentro de la provisión trampa hay una dinamita que explota. Se agrego el efecto del viento en las armas que lo piden y además se agregó un gráfico de viento para saber la potencia y dirección. Se modificó la configuración por yaml del cliente para que lea del mismo archivo que están las armas y mapas, además a ese archivo se le agrego la configuración del server para tener un único archivo de configuración y que sea independiente de quien lo usa. Se creó una clase para configurar los datos del server y sacar valores hardcodeados. Se arreglaron más bugs de conexión a partidas y gráficos en casos bordes. Se volvió a revisar valgrind para que el server no pierda memoria. Se arregló que cuando un gusano es golpeado su "salto" dependa de que tan cerca esta de la explosión y se hizo que la pirueta sea mas alta que el salto para cumplir con la consigna. Se arregló la animación de ataque para que se pueda ver como realiza el ataque. Además se agregaron cheats para bajarle vida al resto de gusanos, empezar sin la cantidad mínima de jugadores y generar provisiones en la posición del gusano de turno.

## Herramientas utilizadas
### IDEs
Tanto Juan Pablo Fresia como Antonella Jazmín Pellegrini realizaron el código en Visual Studio Code, mientras que Tomás Fabrizio Orsi lo realizó en Emacs.

### Documentación
#### Server
Para el servidor nuestras principales fuentes de información fueron la documentación oficial de [box2d](https://box2d.org/) y [iforce2d](https://www.iforce2d.net/) para ver implementaciones de ejemplo.

#### Cliente
Para cliente, se consultaron varios ejemplos y tutoriales de SDL2. Principalmente [libSDL2pp-tutorial](https://github.com/libSDL2pp/libSDL2pp-tutorial), tutorial SDL en [Lazy Foo](https://lazyfoo.net/tutorials/SDL), ejemplos de la librería [libSDL2pp](https://github.com/libSDL2pp/libSDL2pp), y la misma wiki de [SDL2](https://wiki.libsdl.org/SDL2/FrontPage).

### Otras herramientas
Una herramienta que empezamos a usar desde el comienzo del trabajo fueron las "Github Actions". Usando un servidor propio, cada commit era compilado cada vez que se realizaba un push. Esto nos permitía tener un ambiente "agnóstico" para testear cada commit. 
En las github actions incorporamos 3 compiladores distintos: Gcc, Clang y Zig. Esto lo hicimos con la esperanza de que distintos compiladores den distintas advertencias, y así poder mejorar la calidad del código. En la practica, esto no sucedió. Los 3 compiladores tenían, en esencia, el mismo comportamiento. 
Ademas, de compilar el código, corríamos cppcheck. Esto lo hicimos con la esperanza de poder atrapar errores a futuro. Sin embargo, debido a la longitud del TP, terminamos ignorando casi en su totalidad cppcheck.
En retrospectiva, hubiese sido mejor tener solamente los compiladores (Gcc y/o Clang; Zig tuvo problemas de linkeo con algunas de las librerías); ya que, al tener siempre errores con cppcheck, las actions perdían su "peso".


## Puntos problemáticos
### Server
Con diferencia, el punto mas problemático del TP fueron las armas y sus implementaciones. Cada una de estas tenia un comportamiento propio y técnicamente desafiante. Toda la implementación de las explosiones consistió en un minucioso proceso de prueba y error de parámetros hasta encontrar valores que nos satisficiesen. Una decisión que tomamos fue la de tener una clase "Arma" genérica. Esto lo hicimos porque creíamos que seria mas fácil tener una única clase que cambiase de valores en vez de varias clases que implementasen todas las armas. En retrospectiva, tal vez hubiese sido mejor idea hacer una interfaz "Arma" y que cada arma fuese una implementación de dicha interfaz. Esto, potencialmente, podría haber dejado el código mas limpio, ya que cada implementación estaría encapsulado en la clase del arma correspondiente.
Sin embargo, a pesar de estas dificultades, logramos implementar todas las armas.

Relacionado al tema de las armas, las explosiones fueron particularmente problemáticas. Al no usar hitboxes y, en cambio, usar colisiones entre cuerpo debíamos tener en cuenta una gran cantidad de factores en cuenta. Principalmente: cuánto tiempo debían durar estos proyectiles que simulaban la explosión, reconocer cuándo recibir daño para cada gusano (muchos de los proyectiles podían ser de la misma explosión y entonces terminábamos con "falsos positivos") y también saber eliminarlos. Por otro lado, una vez solucionado eso, se nos presentó varias veces el problema de tener que usar bastante tiempo en probar distintos valores para poder llegar al efecto deseado, esto se dio en el salto, pirueta, al recibir daño, moverse y en la trayectoria de los proyectiles.
Otro punto problemático relacionado fue la destrucción de cuerpos de box2d, tanto para dejar de graficarlos como para liberar los recursos.

## Cambios que haríamos
Consideramos que en base a la experiencia de hacer este proyecto, si tuviésemos que volver a hacerlo usaríamos más tiempo al principio para crear una estructura más sólida y que considere features que no implementamos al principio pero que eran necesarias. Por ejemplo, hubo refactors de la clase cliente y jugador que se podrían haber evitado si hubiésemos planteado del principio una estructura más sólida que nos sirva para el proyecto planteado.
Además otro cambio que haríamos sería cambiar el foco de trabajo de "hacerlo para que funcione en la próxima reunión" a "hacerlo pensando en la entrega final" ya que hubo refactors que tuvimos que realizar por el simple hecho tener una implementación temporal porque sino en las reuniones semanales no veíamos avances.

## Incorporaciones al temario
Una de los temas que nos hubiese gustado ver en clase es Box2d. Mucha de la documentación que recomienda la cátedra tiene funcionalidades deprecadas y/o no funcionales. 
Una introducción a como funciona Box2d y como estructurar código al rededor de este hubiese sido de gran ayuda. Con esta información, hubiese sido mas fácil estructurar la lógica de la parte del servidor.
