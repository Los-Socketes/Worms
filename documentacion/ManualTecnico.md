# Cliente

# Protocolo
El protocolo tiene dos tipos de mensajes, los de envío y los de obtención de información. Para ambos la estructura es similar, pero en lo que se diferencian es en que devuelven.\
Los de envío, que comienzan en su mayoría con "enviar" en caso de éxito devuelven true, mientras que en caso de fallar devuelven false. Estos mensajes comienzan enviando un código en base al tipo de mensaje. Internamente, comienzan con un código para identificar el tipo de información a enviar y luego el contenido en un formato específico. Cada tipo de dato se envía siguiendo un formato, siendo este el siguiente:
- Bools, códigos y enums: int8_t. Para los códigos existen métodos genéricos para enviarlos y recibirlos
- Vectores: primero se envía la cantidad de valores a leer y luego cada valor/ conjunto de valores que forme el elemento del vector.
- Floats: se convierten en ints con la función toInt() y se vuelven floats con toFloat(). Ademas, como se envían en int32_t es necesario el uso de htonl y ntohl para poder obtenerlos correctamente.
- Ids: se envían con int32_t, pero también existen métodos para simplificar ese paso.
- Pair: se preparan los valores en un vector y se envían, en caso de ser coordenadas, primero x y luego y.

Por otro lado, para obtener los mensajes se van obteniendo haciendo el proceso inverso a enviar los datos y se almacenan en un struct en base a la firma del método. En caso de que en algún momento falle el método y no consiga obtener toda la información, se devuelve un struct/ valor que es conocido como inválido. Para los ids existe INVAL_ID, para enums que definen el tipo de acción a realizar se agrego un valor que sea INVAL_* para poder diferenciarlos.\
El único método distinto es el de obtenerAccion, ya que es el que debe recibir todos los tipos de acción que el cliente/ jugador le envíe. Por esta misma razón, es necesario contemplar varios códigos validos ya que en esta implementación, los distintos tipos de acción tienen distinto código para que los mensajes sean lo mas cortos posibles y de esta forma la información se transmita eficientemente.

# Server
El server contiene una clase "Server" que inicia al aceptador y espera a que se escriba por la terminal "q" para cerrar el programa. Cada una de estas partidas se corre en un hilo propio. El conjunto de las partidas es manejado a través de un "Manejador de Partidas"; este se encarga de que crear nuevas partidas y destruir el hilo/ partida si la partida finalizó.\
Cada partida de worms es manejada por una clase "Partida". Esta corre un método principal llamada "game loop" (en realidad, el método se llama "run", pero a través de macros se lo renombro para que fuese algo mas representativo). Cada persona que se conecta a una partida es asignada una clase Cliente, la cual tiene una clase Reciever y Sender. Estas ultimas son las que usa la partida para recibir y enviar información en el struct de "EstadoDelJuego".\
Cada partida tiene además un conjunto de Jugadores y Gusanos (los jugadores tienen gusanos asociados para saber su estado en la partida). El gusano de turno actual realiza acciones que le llegan a través de una cola de acciones. Dicha cola recibe las acciones a través de los recievers de cada cliente.\
Los gusanos interactúan fundamentalmente con la clase Arma. Esta es una clase genérica que encapsula el comportamiento de todas las armas la cual cambia sus parámetros dependiendo del arma elegida. La partida accede a dicha arma para determinar que tipo de disparo/ataque realizo el gusano y características de cada arma.

## Clases implementadas
- Server: Inicia el programa y espera una "q" para terminar.
- Monitor de partida: Encargado de manejar todas las partidas vivas. Cuando una de estas termina, este se encarga de destruirla.
- Partida: Encapsula toda la logica del juego. Clase principal de la parte del server. Esta se encarga de hacer que los gusanos ejecuten sus acciones, maneja los turnos y posee toda la logica de box2d.
- Cliente: Representa un usuario conectado al servidor. Posee las clases sender y reciever.
- Sender: Clase que se encarga de enviar el estado del juego a traves del protocolo.
- Reciever: Clase que se encarga de recibir las acciones y enviarselas a la partida. Esta se encarga de incluir el ID del jugador al que representa. 
- Jugador: Representa el jugador de un cliente. Este tiene una lista de sus gusanso y se encarga de saber a que gusano le toca moverse.
- Gusano: Clase que ejecuta las acciones recibidas. Tiene la logica de qué proyectiles crear dada una accion.
- Configuración server: Clase que lee el yaml y actualiza los valores de las constantes usadas. No tiene ningun tipo de logica del juego.
- Aceptador: Encargado de aceptar conexiones mediante un socket aceptador. Este se encarga de crear clientes y destuirlo en caso de que se pierda la conexion.
- Provision: Clase que se encarga de la logica de las provisiones
- Arma: Clase genérica que encapsula el comportamiento de todas las armas. Cambia sus parámetros dependiendo del arma elegida. 
- Mapa: Clase que se encarga de la logica del mapa. 

![Diagrama de clases](diagramaClases.png)
