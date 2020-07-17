---
title: ¡Comienza a jugar!
description: Descarga, instalación y configuración del juego
---

<!-- ##################################################################### -->

# Descarga

Completar descarga. Probando botones:
<a href="{{ site.github.zip_url }}" class="btn">Descargar .zip</a> <a href="{{ site.github.tar_url }}" class="btn">Descargar .tar.gz</a>

Probando lista:
* <a href="{{ site.github.zip_url }}">Descargar .zip</a>.
* <a href="{{ site.github.tar_url }}">Descargar .tar.gz</a>.

<!-- ##################################################################### -->

# Instalación
*Esta sección se encuentra más detallada en el [Manual de Usuario](Manual_de_Usuario.pdf), por lo que se recomienda su lectura en caso de necesitarla.*

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