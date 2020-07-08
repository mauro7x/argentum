# Argentum Online

## Detalles administrativos

* **Materia:** Taller de Programación I
* **Año y cuatrimestre:** 1°C 2020
* **Tutor:** Martín Di Paola

## Equipo de desarrollo

| Nombre y apellido | Padrón | Mail | GitHub |
|-------------------|--------|------|--------|
| Mauro Parafati | 102749 | mparafati@fi.uba.ar | [mauro7x](https://github.com/mauro7x) |
| Santiago Klein | 102192 | sklein@fi.uba.ar | [sankle](https://github.com/sankle) |
| Yuhong Huang | 102146 | yhuang@fi.uba.ar | [tonyhuang07](https://github.com/tonyhuang07) |

## Enlaces de interés
* [**Repositorio en GitHub**](https://github.com/mauro7x/taller_final)
* [**Manual de usuario**](Documents/Manual_de_Usuario.pdf) - Manual para ambos aplicativos, incluyendo requerimientos de hardware, software, instalación, configuración y uso.

### Documentacin en construcción
Los siguientes documentos se encuentran en elaboración y se adjunta el link para edición a los mismos en Overleaf:
* [**Manual de Proyecto**](https://es.overleaf.com/8387857119mbhmdtbrpvnh) - Organización, cronogramas, división de tareas, y evolución del proyecto.
* [**Documentación Técnica**](https://es.overleaf.com/2395968991ysqtxgxxsncs) - Detalles de implementación y de diseño, estructuras utilizadas, herramientas integradas.

## Instalación
*(Esta sección se encuentra detallada en el [Manual de Usuario](Documents/Manual_de_Usuario.pdf), por lo que se recomienda su lectura.)*

Para instalar el juego se provee de un script `installer.sh` que corre en bash. Se debe ejecutar:
```bash
./installer.sh
```

Si fallan los permisos, se le deberán conceder los mismos ejecutando:
```bash
chmod +x ./installer.sh
```

El script ofrece un menú con múltiples opciones. Para instalar todo, simplemente elegir la opción `a`.

## Configuración
*(Esta sección se encuentra detallada en el [Manual de Usuario](Documents/Manual_de_Usuario.pdf), por lo que se recomienda su lectura.)*

Se provee de un script de configuración, que permite modificar distintos valores numéricos del juego. Para esto, se debe ejecutar:
```bash
./config.sh
```

Al igual que con el script del instalador, si fallan los permisos, se le deberán conceder los mismos ejecutando:
```bash
chmod +x ./config.sh
```

Al abrirlo, podremos seleccionar el archivo que queremos editar, y el mismo se abrirá para su edición utilizando `Vim`.
