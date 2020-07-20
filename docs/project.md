---
title: Información del Proyecto
description: Detalles administrativos, equipo de desarrollo, documentación, extensiones
---

<!-- ##################################################################### -->

# Descripción

Remake del juego [**Argentum Online**](https://github.com/ao-libre) para sistemas operativos basados en Debian, utilizando programación concurrente orientada a objetos y sockets TCP/IPv4 bloqueantes. Trabajo final de la materia **Taller de Programación I** para la carrera Ingeniería Informática de la Universidad de Buenos Aires.

<!-- ##################################################################### -->

# Detalles administrativos
* **Materia:** Taller de Programación I
* **Año y cuatrimestre:** 1°C 2020
* **Tutor:** Martín Di Paola

## Equipo de desarrollo

| Nombre y apellido | Padrón | Mail | GitHub |
|-------------------|--------|------|--------|
| Mauro Parafati | 102749 | mparafati@fi.uba.ar | [mauro7x](https://github.com/mauro7x) |
| Santiago Klein | 102192 | sklein@fi.uba.ar | [sankle](https://github.com/sankle) |
| Yuhong Huang | 102146 | yhuang@fi.uba.ar | [tonyhuang07](https://github.com/tonyhuang07) |

<!-- ##################################################################### -->

# Documentación
* [**Manual de Usuario**](Manual_de_Usuario.pdf) - *Manual para ambos aplicativos (cliente y servidor), incluyendo requerimientos de hardware, software, instalación, configuración y uso.*
* [**Manual de Proyecto**](Manual_de_Proyecto.pdf) - *Organización, cronogramas, división de tareas, y evolución del proyecto.*
* [**Manual de Referencia**]() - *Detalles de implementación y de diseño, estructuras utilizadas, herramientas integradas.*

<!-- ##################################################################### -->

# Features a agregar

Si bien se intentó agregar la mayor cantidad de features al juego, por una cuestión de tiempo no pudimos llegar a implementar todas las ideas que teníamos en mente, por lo que tuvimos que priorizar algunas por sobre otras. Se listan a continuación posibles mejoras para realizar en un futuro:

## Modelo
* **Refactorizar el sistema de movimiento en el cliente.** Actualmente, se utilizó un sistema basado en tiempo real para determinar cuántos pixeles debe avanzar una unidad. Si bien funciona correctamente, en caso de que la función para obtener el tiempo tenga algún tipo de delay, esto podría notarse. Para optimizar el mismo, se propone utilizar las iteraciones del cliente como medida de tiempo transcurrido para avanzar la posición de la unidad en el mapa.

* **Scrolling en la consola.** Mejora que consideramos de muy baja prioridad, puesto que no imposibilita ningún accionar en el juego, simplemente agrega comodidad.

* **Dividir algunos archivos de configuración.** Por ejemplo, el archivo de configuración del cliente (`config.json`) es demasiado grande y tiene mucha información. Se podría separar el mismo en distintos sub-archivos para cada parte del cliente.

## Jugabilidad
* **Indicador que diga la posición actual en una esquina.** No está bueno estar perdido en un mapa de 200x200 tiles. Bah depende.

* **Vista previa de información de los objetos.** Mejora de jugabilidad. La idea consiste en que cuando se pase el mouse por encima de un objeto, se nos muestre una pequeña ventana con información sobre el mismo.

* **Indicador que diga la posición actual en una esquina.** Mejora de muy baja prioridad para ganar jugabilidad.
    
* **Globo de mensajes generales.** Según el mapa, se pueden encontrar fondos que dificulten la lectura de los mensajes. Se propone para solucionar este problema, el uso de globos que contengan los mensajes y contraste con los mismos para facilitar la misma en cualquier contexto.

<!-- ##################################################################### -->
