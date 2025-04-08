# Diseño de un Sistema de Archivos Simple

Este documento describe un sistema de archivos simplificado en el que se gestiona el almacenamiento de archivos a partir de un nodo raíz (`Root`). Cada archivo tiene un nombre, un identificador de inicio (`ID_inicio`) y un identificador de fin (`ID_fin`) que permiten ubicar su posición dentro de una región de almacenamiento.

## Estructura del Sistema

### Root

- Es el nodo principal del sistema.
- Contiene un `vector<archivos>` que representa la lista de archivos gestionados.
- No almacena directamente los archivos, pero sí mantiene la referencia a ellos.

### Archivos

Cada archivo está representado por una estructura que incluye:

- `Nombre`: una cadena de hasta 20 caracteres.
- `ID_inicio`: identificador del primer bloque del archivo.
- `ID_fin`: identificador del último bloque del archivo.

Adicionalmente, cada archivo almacena los siguientes atributos como caracteres:

- Nombre (hasta 20 caracteres).
- Inicio (2 caracteres).
- Fin (2 caracteres).

Estos se almacenan como texto y forman parte del contenido del archivo en el bloque.

## Asignación de Espacio

- Cada bloque tiene un tamaño fijo de **256 bytes**.
- El espacio de almacenamiento está representado como una lista continua de bloques, donde cada archivo ocupa uno o más bloques consecutivos según su tamaño.
- Los archivos se almacenan en los bloques secuenciales a partir de la raíz, comenzando en el bloque 0 (reservado para el `Root`).

### Ejemplo

Supongamos los siguientes archivos:

| Archivo     | ID_inicio | ID_fin |
|-------------|-----------|--------|
| Ballena.txt | 1         | 1      |
| Gato.txt    | 2         | 2      |
| Perro.txt   | 3         | 3      |
| Casa.txt    | 4         | 4      |

Estos archivos ocuparían los bloques de almacenamiento de la siguiente forma:

- **Root** → bloque 0
- **Ballena.txt** → bloque 1 (ID 256)
- **Gato.txt** → bloque 2 (ID 512)
- **Perro.txt** → bloque 3 (ID 768)
- **Casa.txt** → bloque 4 (ID 1024)
- **Archivo N** → bloque N (ID N * 256)

Si un archivo excede los 256 caracteres de capacidad, se almacenará en varios bloques consecutivos. En ese caso, `ID_fin` será mayor que `ID_inicio` y se reservará un rango de bloques para su almacenamiento.

## Consideraciones

- El vector de archivos solo existe en `Root`.
- Cada archivo tiene su espacio designado y no puede ser compartido.
- El diseño permite expansión si un archivo requiere más de un bloque.
- El sistema es secuencial y no admite almacenamiento fragmentado fuera del rango asignado (`ID_inicio` a `ID_fin`).
