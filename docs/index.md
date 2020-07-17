---
title: Argentum Online
description: Juego multijugador online donde los participantes controlan un personaje de rol en un mundo fantástico de magia y lleno de criaturas salvajes.
---

<!-- ##################################################################### -->

# Contenidos

1. [Proyecto](#proyecto)
    * [Detalles administrativos](#detalles)
    * [Equipo de desarrollo](#equipo)
    * [Documentación](#documentacion)
2. [Juego](#juego)
    * [Trailer](#trailer)
    * [Capturas de pantalla](#capturas)
3. [Instalación](#instalacion)
    * [Configuración](#configuración)
    * [Scripts](#scripts)

<!-- ##################################################################### -->

# Proyecto <a name="proyecto"></a>

Remake del juego [**Argentum Online**](https://github.com/ao-libre) para sistemas operativos basados en Debian, utilizando programación concurrente orientada a objetos y sockets TCP/IPv4 bloqueantes. Trabajo final de la materia **Taller de Programación I** para la carrera Ingeniería Informática de la Universidad de Buenos Aires.

## Detalles administrativos <a name="detalles"></a>
* **Materia:** Taller de Programación I
* **Año y cuatrimestre:** 1°C 2020
* **Tutor:** Martín Di Paola

## Equipo de desarrollo <a name="equipo"></a>

| Nombre y apellido | Padrón | Mail | GitHub |
|-------------------|--------|------|--------|
| Mauro Parafati | 102749 | mparafati@fi.uba.ar | [mauro7x](https://github.com/mauro7x) |
| Santiago Klein | 102192 | sklein@fi.uba.ar | [sankle](https://github.com/sankle) |
| Yuhong Huang | 102146 | yhuang@fi.uba.ar | [tonyhuang07](https://github.com/tonyhuang07) |

## Documentación <a name="documentación"></a>
* [**Manual de usuario**](Manual_de_Usuario.pdf) - *Manual para ambos aplicativos (cliente y servidor), incluyendo requerimientos de hardware, software, instalación, configuración y uso.*

### En construcción
Los siguientes documentos se encuentran en elaboración y se adjunta el link para edición a los mismos en Overleaf:
* [**Manual de Proyecto**](https://es.overleaf.com/8387857119mbhmdtbrpvnh) - *Organización, cronogramas, división de tareas, y evolución del proyecto.*
* [**Manual de Referencia**](https://es.overleaf.com/2395968991ysqtxgxxsncs) - *Detalles de implementación y de diseño, estructuras utilizadas, herramientas integradas.*

<!-- ##################################################################### -->

# Juego <a name="juego"></a>

Insertar una breve descripción del juego.

## Trailer <a name="tailer"></a>

Insertar video trailer del juego.

## Capturas de pantalla <a name="capturas"></a>

Insertar capturas de pantalla del juego.

<!-- ##################################################################### -->

# Instalación <a name="instalacion"></a>
*(Esta sección se encuentra detallada en el [Manual de Usuario](Manual_de_Usuario.pdf), por lo que se recomienda su lectura.)*

Para instalar el juego se provee del script `installer.sh` que corre en bash. Se debe ejecutar:
```bash
./installer.sh
```

Si fallan los permisos, se le deberán conceder los mismos ejecutando:
```bash
chmod +x ./installer.sh
```

El script ofrece un menú con múltiples opciones. Para instalar todo, simplemente elegir la opción `a`.

## Configuración <a name="configuracion"></a>
Se provee de un script de configuración, que permite modificar distintos valores numéricos del juego. Para esto, se debe ejecutar:
```bash
./config.sh
```

Al igual que con el script del instalador, si fallan los permisos, se le deberán conceder los mismos ejecutando:
```bash
chmod +x ./config.sh
```

Al abrirlo, podremos seleccionar el archivo que queremos editar, y el mismo se abrirá para su edición utilizando `Vim`.

## Scripts <a name="scripts"></a>
También se provee de un script de *cheats* que permite cargar configuraciones determinadas de manera rápida, como por ejemplo que los jugadores empiecen con objetos, o demás. Para probar los distintos *cheats* disponibles, se debe ejecutar:
```bash
./scripts.sh
```

Nuevamente, si fallan los permisos, se le deberán conceder los mismos ejecutando:
```bash
chmod +x ./scripts.sh
```

<!-- ##################################################################### -->
