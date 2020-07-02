# To-Do List

## Server

* ~~Cuando manejemos el tema de la database, es importante acordarse de incluir alguna verificación para que un usuario conectado (un nickname/pass conectado en el server) no pueda volver a usarse para conectarse. -Mau, 21/06.~~
    * **UPDATE:** done. -Mau, 28/06.

* Crear clase Map (con algún nombre del modelo) para el server que sea exclusiva del mismo y que encapsule el MapContainer que ahora tiene el game, y los métodos que están en Map common y que en realidad son exclusivos del server. Con los métoods occupyTile() y clearTile() de common map debería ser suficiente. -Mau, 21/06.
    * low-priority.

---

## Client

* Realizar el movimiento de las unidades utilizando los ticks de la iteración de acten vez del tiempo interno que haya pasado. -Mau, 21/06.
    * Medium-priority.

* ~~Eliminar SDLException y usar Exception con SDL_GetError(). -Mau, 21/06.~~
    * **UPDATE:** Done. -Mau, 28/06.

* ~~Cambiar el wait primitivo por el primer paquete y la lógica del mismo del receiver a una clase aparte, y no lanzar ningun hilo hasta que el mismo no llegue. -Mau, 21/06.~~
    * **UPDATE:** Ahora esto no se hace más, sino que se tiene un LogIn que se lanza antes de los hilos del receiver y del dispatcher, con el objetivo de handlear la conexión del jugador al juego en sí (una vez que ya se conectó el socket) y de recibir el primer paquete (la data inicial). Esta abstracción permite convertirla en una vista en un futuro. -Mau, 22/06.

* ~~Agregarle el nickname en la cabeza a los jugadores. -Mau, 23/06.~~
    * **UPDATE:** Done. -Mau, 26/06.

* Optimizar el renderizado del mapa usando vectores de listas para los objetos estáticos (recomendación de Martín). -Mau, 23/06.
    * Medium-priority.

* ~~Implementar la consola de output (la de input ya funciona). -Mau, 23/06.~~
    * **UPDATE:** done! -Mau, 24/06.

* ~~Agregar tick que titila al escribir en la consola. -Mau, 23/06.~~
    * **UPDATE:** Lo hice jeje. -Mau, 24/06.

* Indicador que diga la posición actual en una esquina. -Mau, 23/06.
    * low.

* Pensar si vale la pena tener los métodos init o usar el constructor. -Mau, 24/06.
    * low.

* Una clase Units que encapsule los contenedores de unidades de criaturas, personajes, etc. -Mau, 24/06.
    * low.

* ~~Idea primitiva: inicializar los sistemas de SDL una sola vez (para la vista de login y para la del game) en Client.cpp con alguna función, por lo que en GameView podemos eliminar la función init y hacer la inicialización leyendo los jsons en el constructor. -Mau, 24/06.~~
    * **UPDATE:** Done. -Mau, 27/06.

* ~~Hacer alguna animación cuando clickeo un objeto del inventario (selección + doble click). -Mau, 25/06.~~
    * **UPDATE:** done. -Mau, 26/06.

* Separar config.json en varios. (homeview_config.json, etc...). -Mau, 28/06.
    * low.

* ~~Poder tomar elementos haciendo click en el tile que los contiene. -Santi ;)~~
    * ~~Ponerle un shortcut.~~
    * **UPDATE:** done! -Mau, 01/07.

* Agregar el mensaje del chat general en la cabeza del character por un periodo breve de tiempo. -Mau, 30/06.
    * low.

---

## Common

* ~~En muchos de los usos que le di a unordered_map habilité el constructor por movimiento porque no sabía que con emplace se podía evitar. Cambiar esto para todos los casos donde realmente no sea necesario moverlo. -Mau, 21/06.~~
    * **UPDATE:** done! -Mau, 30/06.

* ~~Chat general. Cuando un jugador escribe algo, se debe broadcastear a todos como una notification reply que va en el chat gral. -Mau, 23/06.~~
    * **UPDATE:** ez pz. 

* Endianness en el socket. -Santi, 25/06.
    * high.

* Agregarle a los items wearable un campo "info" que cuando el cliente les haga click derecho se imprima por consola. -Mau, 28/06.
    * low.

## Cosas que faltan (meeting 30/06).

* Movimiento inteligente de monstruos y ataque.
    * high.

* Soporte para la interacción con:
    * npc-banco: sistema bancario (high).
    * npc-sacerdote: curar, resucitar (high). resucitar desde lejos (medium).
    * npc-comerciante: sistema compra-venta (high).
        * distintos comerciantes: low.

* Data inicial configurable en un json (high).

* Frame-rate del cliente (medium), tick-rate del servidor (medium) en jsons.

* ~~Sonidos de eventos (medium).~~
    * **UPDATE:** done. -Mau, 02/07.

* Instalador.



---

