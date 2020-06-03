# Trabajo Práctico Final

## Detalles administrativos

* **Materia:** Taller de Programación I
* **Año y cuatrimestre:** 1°C 2020
* **Tutor:** Martín Di Paola

## Integrantes

| Nombre y apellido | Padrón | Mail | GitHub |
|-------------------|--------|------|--------|
| Mauro Parafati | 102749 | mparafati@fi.uba.ar | [mauro7x](https://github.com/mauro7x) |
| Santiago Klein | 102192 | sklein@fi.uba.ar | [sankle](https://github.com/sankle) |
| Yuhong Huang | 102146 | yhuang@fi.uba.ar | [tonyhuang07](https://github.com/tonyhuang07) |
| Francisco Gauna | 100563 | fgauna@fi.uba.ar | [FranciscoGauna](https://github.com/FranciscoGauna) |

## Enlaces de interés
* [**Repositorio en GitHub**](https://github.com/mauro7x/taller_final)

<!-- ###################################################################### -->

# Compilar con CMake

Para compilar, se deben ejecutar desde la raíz del repositorio las siguientes líneas:

```
mkdir build && cd build
cmake ..
```

Una vez hecho esto y si no hubieron errores, obtendremos el `Makefile` generado por **CMake**. Ahora podemos:

* Para compilar el **servidor**:
    ```
    make server
    ```
* Para compilar el **cliente**:
    ```
    make client
    ```
* Para compilar todo:
    ```
    make
    ```

<!-- ###################################################################### -->

---
