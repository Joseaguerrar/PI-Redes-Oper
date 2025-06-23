# Proyecto: Descubrimiento Tenedor/Figuras

---

## Estructura del Proyecto

```
.
├── exec/                    # Ejecutables generados tras compilación
├── obj/                     # Archivos objeto intermedios
├── figura_discovery.cpp    # Código fuente del servidor de figuras
├── tenedor_discovery.cpp   # Código fuente del tenedor (cliente descubridor + HTTP)
├── Socket.h / Socket.cpp   # Implementación de clase Socket basada en VSocket
├── VSocket.h / VSocket.cpp # Clase base VSocket para comunicación en red
├── file_system.hpp / .cpp  # Módulo para manejo y búsqueda de figuras ASCII
├── Makefile                # Script de compilación con targets específicos
└── README.md               # Este manual
```

---

## Compilación

Se utiliza `make` con los siguientes objetivos:

- `make` → Compila todos los ejecutables
- `make figuras` → Solo compila el servidor de figuras
- `make tenedor` → Solo compila el tenedor
- `make clean` → Elimina todos los objetos y ejecutables generados

**Requisitos:**

- Sistema Linux
- Compilador `g++`
- Permisos de red adecuados (para usar sockets UDP y TCP)

---

## Flujo del Sistema

1. El **Tenedor** inicia un hilo que periódicamente (cada 120 segundos):
   - Envía un mensaje UDP tipo `"GET /servers"` a direcciones de broadcast.
   - Escucha respuestas por 5 segundos.

2. El **Servidor de Figuras**:
   - Está escuchando en el puerto UDP 5353.
   - Al recibir `"GET /servers"`, responde con un mensaje que contiene:

     ```
     <nombre_servidor> | <ip> | <lista_figuras>
     ```

   - También escucha en el puerto TCP 8081 para solicitudes de figuras.

3. El **Tenedor**:
   - Recoge los nombres de las figuras anunciadas por cada servidor y construye una tabla de ruteo.
   - También ejecuta un servidor HTTP en el puerto 8080.

4. Un **cliente externo (curl, navegador, etc.)** puede enviar solicitudes HTTP al Tenedor para obtener figuras específicas.

---

## Ejecución

### 1. Levantar el servidor de figuras

```bash
./exec/figura_discovery.out
```

- Carga figuras desde el sistema de archivos (almacenadas localmente).
- Responde a descubrimientos por UDP en el puerto **5353**.
- Atiende solicitudes TCP en el puerto **8081**.

### 2. Levantar el tenedor

```bash
./exec/tenedor_discovery.out
```

- Envía mensajes de descubrimiento por UDP a `127.0.0.255` (o IP de broadcast real).
- Escucha en el puerto **8080** como servidor HTTP.
- Cuando un cliente solicita una figura por HTTP, busca la IP en su tabla de ruteo y contacta al servidor de figuras vía TCP.

---

## Prueba usando `curl`

Una vez ambos programas estén corriendo, puede simular un cliente desde otra terminal con:

```bash
curl http://localhost:8080/figure?name=gato
```

Esto generará:

- Una solicitud HTTP al Tenedor.
- El Tenedor buscará la IP asociada a "gato".
- Contactará al servidor de figuras por TCP y pedirá la figura con:

  ```
  GET /figure/gato
  ```

- El servidor de figuras responderá con el arte ASCII.
- El Tenedor lo devuelve formateado como HTML dentro del `<pre>` para visualización.

Si la figura no se encuentra:

- El tenedor muestra mensaje de que no se encontró la figura en la tabla de ruteo.

---

## Consideraciones de Red

- Para pruebas locales, el broadcast se realiza a `127.0.0.255`.
- En una red real, debe usarse la IP de broadcast real. Usa `ip addr` para averiguarla, por ejemplo:

```bash
ip addr show
```

Y busque la línea:

```
inet 172.16.123.81/28 brd 172.16.123.95 ...
```

Cambia `127.0.0.255` por `172.16.123.95` en `broadcast_ips` del `tenedor_discovery.cpp`.

---

## Formato del Mensaje de Descubrimiento

**Solicitud (desde Tenedor al broadcast UDP):**

```
GET /servers
```

**Respuesta (desde servidor de figuras):**

```
<nombre_servidor> | <ip> | <figura1,figura2,...>
```

Ejemplo:

```
ServidorA | 127.0.0.1 | gato,barco,arbol_navidad,sombrilla
```
