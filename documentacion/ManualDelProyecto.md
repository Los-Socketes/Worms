# Manual del proyecto
## División del trabajo
El proyecto se dividió en:
- Cliente (realizado por Juan Pablo Fresia)
- Server (realizado por Tomás Fabrizio Orsi)
- Protocolo + concurrencia (realizado por Antonella Jazmín Pellegrini)

## División semanal
### Semana 1
Se pidió tener una conexión funcional entre server y cliente, se podía demostrar con el movimiento del gusano. \
Se presentó un esqueleto con conversación inicial entre server y cliente para crear una partida. Además, del lado del cliente, se presentó que SDL mostraba una pantalla, renderizaba y se registraba el uso de las flechas.

### Semana 2
Se pidió tener un gusano moviendose y configuración con archivos yaml. Además, se pidió reestructurar el proyecto para que cliente y jugador sean una sola clase. \
Se presentó un gusano con animación de movimiento (ya con movimiento enviado por socket) y el refactor pedido.

### Semana 3
Se pidió tener implementada alguna arma para verificar lógica de bajar vida, incluir box2d, tener la concurrencia resuelta y reestructurar el proyecto para que el comportamiento de jugador vuelva a estar dividido en 2 clases (cliente y jugador).\
Se presentó un juego en el que se podían conectar mas de 1 gusano a la misma partida. Se implementó gravedad, movimiento y colisión con box2d. Se presentó el mapa renderizado en capas con fondo, agua y gusanos, además de movimiento de cámara y animaciones para equipar bate y atacar. De parte del protocolo se presentaron tests para todos los metodos públicos además de cambios para permitir los avances de cliente y servidor. Por último, se presentó una base de cómo se representarán las armas y el archivo de configuración de las mismas.

### Semana 4
Se pidió tener implementadas 3 armas mas, además de poder tenerlas en un menú. Además del lado del cliente se pidió agregar el manejador de texturas. Para el server se pidió tener un juego "funcional" de forma que interactuen los gusanos y se puedan atacar los unos a los otros. \
Se presentó un menú con todas las armas que se pueden equipar. Se conectan varios gusanos, se golpean entre ellos y se baja la vida. Para el bate se puede setear el ángulo y tiene los límites correctos. Se presentó una base de teletransportación que funciona pero envía una posición constante. De parte del protocolo se modificó el envío y obtención del estado del juego para que sea con shared pointers, también se agregaron los mensajes para la configuración de ataque (ángulo, potencia, cuenta regresiva, coordenadas teletransportación).

### Semana 5
Se pidió tener implementadas más armas con hitboxes (u otra forma, si se puede hacer). Modificar cómo se aplica el daño para que la entidad sepa con que esta colicionando y cómo afecta la otra entidad. Se pide agregar readme con instrucciones de ejecución para la entrega parcial. También se pide tener el menú con QT implementado y poder eliminar los elementos de box2d.

## Herramientas
(Poner los IDES)

Una herramienta que empezamos a usar desde el comienzo del trabajo fueron las "Github Actions". Usando un servidor propio, cada commit era compilado cada vez que se realizaba un push. Esto nos permitia tener un ambiente "agnostico" para testear cada commit. 
En las github actions incorporamos 3 compiladores distintos: Gcc, Clang y Zig. Esto lo hicimos con la esperanza de que distintos compiladores den distintas advertencias, y asi poder mejorar la calidad del codigo. En la practica, esto no sucedio. Los 3 compiladores tenian, en esencia, el mismo comportamiento. 
Ademas, de compilar el codigo, corriamos cppcheck. Esto lo hicimos con la esperanza de poder atrapar errores a futuro. Sin embargo, debido a la longitud del TP, terminamos ignorando casi en su totalidad cppcheck.
En retrospectiva, hubiese sido mejor tener solamente los compiladores (Gcc y/o Clang; Zig tuvo problemas de linkeo con algunas de las librerias); ya que, al tener siempre errores con cppcheck, las actions perdian su "peso".
