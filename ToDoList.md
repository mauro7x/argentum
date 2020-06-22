# To-Do List

## Server

* Cuando manejemos el tema de la database, es importante acordarse de incluir alguna verificación para que un usuario conectado (un nickname/pass conectado en el server) no pueda volver a usarse para conectarse. -Mau, 21/06.
* Crear clase Map (con algún nombre del modelo) para el server que sea exclusiva del mismo y que encapsule el MapContainer que ahora tiene el game, y los métodos que están en Map common y que en realidad son exclusivos del server. Con los métoods occupyTile() y clearTile() de common map debería ser suficiente. -Mau, 21/06.

---

## Client

* Realizar el movimiento de las unidades utilizando los ticks de la iteración de acten vez del tiempo interno que haya pasado. -Mau, 21/06.
* Eliminar SDLException y usar Exception con SDL_GetError(). -Mau, 21/06.
* Cambiar el wait primitivo por el primer paquete y la lógica del mismo del receiver a una clase aparte, y no lanzar ningun hilo hasta que el mismo no llegue. -Mau, 21/06.

---

## Common

* En muchos de los usos que le di a unordered_map habilité el constructor por movimiento porque no sabía que con emplace se podía evitar. Cambiar esto para todos los casos donde realmente no sea necesario moverlo. -Mau, 21/06.


---

