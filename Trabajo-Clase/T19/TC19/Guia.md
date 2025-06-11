# Proyecto: Descubrimiento Tenedor/Figuras

---

## Estructura del Proyecto

```
.
├── exec/                    # Ejecutables generados tras compilación
├── obj/                     # Archivos objeto intermedios
├── figura_discovery.cpp    # Código fuente del servidor de figuras
├── tenedor_discovery.cpp   # Código fuente del tenedor (cliente descubridor)
├── Socket.h / Socket.cpp   # Implementación de clase Socket basada en VSocket
├── VSocket.h / VSocket.cpp # Clase base VSocket para comunicación en red
├── Makefile                # Script de compilación con targets específicos
└── README.md               # Este manual
```

---

## Compilación

Se utiliza `make` con los siguientes objetivos:

- `make` → Compila ambos ejecutables (`figura_discovery.out` y `tenedor_discovery.out`)
- `make figuras` → Solo compila el servidor de figuras
- `make tenedor` → Solo compila el tenedor
- `make clean` → Limpia archivos objeto y ejecutables

**Requisitos:**

- Sistema Linux
- Compilador `g++`
- Permisos de red adecuados

---

## Ejecución

> **Importante:** Ambos programas deben ejecutarse dentro de la red asignada: `172.16.123.80/28`.

### 1. Ejecutar el servidor de figuras

```bash
./exec/figura_discovery.out
```

Este componente:

- Envia 3 mensajes de broadcast con su nombre, IP, puerto y figuras disponibles.
- Usa como IP de broadcast: `172.16.123.95`
- Usa el puerto UDP: `8081`

### 2. Ejecutar el tenedor

```bash
./exec/tenedor_discovery.out
```

Este componente:

- Escucha en el puerto UDP `8081`
- Recibe y muestra los mensajes de descubrimiento enviados por el servidor de figuras

Ambos pueden ejecutarse en orden indistinto (el primero que arranque esperará al otro).

---

## Prueba en red local

Si se desea ejecutar en una red distinta a la asignada (por ejemplo, pruebas locales):

- Cambiar temporalmente la IP de broadcast en `figura_discovery.cpp` y `tenedor_discovery.cpp`
- Usar la IP de broadcast de la red local, para esto ejecute el comando:

```bash
ip addr
```

y busque la IP de broadcast de la red local, por ejemplo `172.18.15.255` si estás en la red `172.18.11.204/20`

---

## Formato de mensaje

```
FIGURA_SERVER <nombre_servidor> <ip> <puerto> <lista_figuras>
```

Ejemplo:

```
FIGURA_SERVER ServidorA 172.16.123.85 8081 gato,arbol,sombrilla
```