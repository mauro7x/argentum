---
title: ¡Comienza a jugar!
description: Requerimientos, descarga, instalación y configuración del juego
---

<!-- ##################################################################### -->

# Requerimientos mínimos

Falta completar requerimientos mínimos.

<!-- ##################################################################### -->

# Descarga

Para comenzar, debes descargar el juego (incluye ambos aplicativos: cliente y servidor).
* <a href="{{ site.github.zip_url }}">Descargar en formato .zip (~XX MB)</a>.
* <a href="{{ site.github.tar_url }}">Descargar en formato .tar.gz (~XX MB)</a>.

<!-- ##################################################################### -->

# Instalación

Para instalar el juego se provee del script `installer.sh` que corre en bash. Se debe ejecutar:
```bash
$ ./installer.sh
```

Si fallan los permisos, se le deberán conceder los mismos ejecutando:
```bash
$ chmod +x ./installer.sh
```

El script ofrece un menú con múltiples opciones. Para instalar todo, simplemente elegir la opción `a`. Una vez que la instalación haya finalizado, podrás ejecutar
```bash
$ argentum
```
para abrir el cliente, o
```bash
$ ao-server <puerto>
```
para abrir un servidor de Argentum Online en el puerto provisto como parámetro.

<!-- ##################################################################### -->

# Configuración <a name="configuracion"></a>
Se provee de un script de configuración, que permite modificar distintos valores numéricos del juego. Para esto, se debe ejecutar:
```bash
$ ./config.sh
```

Al igual que con el script del instalador, si fallan los permisos, se le deberán conceder los mismos ejecutando:
```bash
$ chmod +x ./config.sh
```

Al abrirlo, podremos seleccionar el archivo que queremos editar, y el mismo se abrirá para su edición utilizando `Vim`.

## Scripts <a name="scripts"></a>
También se provee de un script de *cheats* que permite cargar configuraciones determinadas de manera rápida, como por ejemplo que los jugadores empiecen con objetos, o demás. Para probar los distintos *cheats* disponibles, se debe ejecutar:
```bash
$ ./scripts.sh
```

Nuevamente, si fallan los permisos, se le deberán conceder los mismos ejecutando:
```bash
$ chmod +x ./scripts.sh
```

<!-- ##################################################################### -->

# Más información

Si tuviste algún problema durante el proceso, en el [Manual de Usuario](Manual_de_Usuario.pdf) se provee una explicación mucho más detallada y con imágenes, por lo que se recomienda su lectura en caso de necesitarla. En este manual también se incluye un pequeño **tutorial** de juego para que comiences tu aventura.

<!-- ##################################################################### -->
