# Cliente

# Protocolo

# Server
El server contiene una clase "Server" que inicia al aceptador y espera a que se escriba por la terminal "q" para cerrar el programa. Cada una de estas partidas se corre en un hilo propio. El conjunto de las partidas es manejado a través de un "Manejador de Partidas"; este se encarga de que crear nuevas partidas y destruir el hilo/ partida si la partida finalizó.\
Cada partida de worms es manejada por una clase "Partida". Esta corre un método principal llamada "game loop" (en realidad, el método se llama "run", pero a través de macros se lo renombro para que fuese algo mas representativo). Cada persona que se conecta a una partida es asignada una clase Cliente, la cual tiene una clase Reciever y Sender. Estas ultimas son las que usa la partida para recibir y enviar información en el struct de "EstadoDelJuego".\
Cada partida tiene además un conjunto de Jugadores y Gusanos (los jugadores tienen gusanos asociados para saber su estado en la partida). El gusano de turno actual realiza acciones que le llegan a través de una cola de acciones. Dicha cola recibe las acciones a través de los recievers de cada cliente.\
Los gusanos interactúan fundamentalmente con la clase Arma. Esta es una clase genérica que encapsula el comportamiento de todas las armas la cual cambia sus parámetros dependiendo del arma elegida. La partida accede a dicha arma para determinar que tipo de disparo/ataque realizo el gusano y características de cada arma.

## Clases implementadas
- Server: Inicia el programa y espera una "q" para terminar.
- Monitor de partida: Encargado de manejar manejar todas las partidas vivas. Cuando una de estas termina, este se encarga de destruirla.
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

