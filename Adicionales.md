# Adicionales (post primer-entrega)

En este archivo se listan ideas que se fueron teniendo a lo largo del desarrollo y que pueden ser implementadas, ya sea para la entrega final o para un trabajo posterior a la entrega.

---

## Server

* Crear clase Map (con algún nombre del modelo) para el server que sea exclusiva del mismo y que encapsule el MapContainer que ahora tiene el game, y los métodos que están en Map common y que en realidad son exclusivos del server. Con los métoods occupyTile() y clearTile() de common map debería ser suficiente. -Mau, 21/06.

---

## Cliente

* Realizar el movimiento de las unidades utilizando los ticks de la iteración de acten vez del tiempo interno que haya pasado. -Mau, 21/06.

* Optimizar el renderizado del mapa usando vectores de listas para los objetos estáticos. -Mau, 23/06.

* Indicador que diga la posición actual en una esquina. -Mau, 23/06.

* Separar config.json en varios. (homeview_config.json, etc...). -Mau, 28/06.

---

## Common

* Agregarle a los items wearable un campo "info" que cuando el cliente les haga click derecho se imprima por consola. -Mau, 28/06.

---