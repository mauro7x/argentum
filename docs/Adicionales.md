# Adicionales (post primer-entrega)

En este archivo se listan ideas que se fueron teniendo a lo largo del desarrollo y que pueden ser implementadas, ya sea para la entrega final o para un trabajo posterior a la entrega.

---

## Server

* Crear clase Map (con algún nombre del modelo) para el server que sea exclusiva del mismo y que encapsule el MapContainer que ahora tiene el game, y los métodos que están en Map common y que en realidad son exclusivos del server. Con los métoods occupyTile() y clearTile() de common map debería ser suficiente.

---

## Cliente

* Realizar el movimiento de las unidades utilizando los ticks de la iteración de acten vez del tiempo interno que haya pasado. -Mau, 21/06.

* Optimizar el renderizado del mapa utilizando estructuras más complejas.

* Indicador que diga la posición actual en una esquina.

* Separar config.json en varios. (homeview_config.json, etc...).

---

## Common

* Agregarle a los items wearable un campo "info" que cuando el cliente les haga click derecho se imprima por consola.

---