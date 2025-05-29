#  Manual de Ejecución: Servidor y Cliente NachOS (`nachitos`)

Este manual explica cómo ejecutar el servidor C++ y luego hacer que el cliente NachOS (`nachitos`) se conecte a él, usando sockets.

---

## Paso 1: Ejecutar el servidor

### 1.1. Entrar al directorio del servidor

```bash
cd code/server
```

1.2. Compilar el servidor (si no lo ha hecho)

```bash
make
```

Esto genera el ejecutable en ./exec/ThreadMirrorServer.out.

1.3. Ejecutar el servidor

```bash
./exec/ThreadMirrorServer.out
```

El servidor se quedará esperando conexiones en el puerto 1234.

 Asegúrese de que el puerto 1234 no esté ocupado por otro proceso.

## Paso 2: Ejecutar el cliente NachOS

### 2.1. Abrir una nueva terminal

Esto es importante porque el servidor sigue corriendo en la anterior.

### 2.2. Ir al directorio del simulador

```bash
cd code/userprog
```

### 2.3. Ejecutar el cliente

```bash
./nachos -x ../test/cliente
```

Este comando hace que NachOS cargue el programa cliente, que se conecta al servidor por socket, envía la petición HTTP y muestra la respuesta (por ejemplo, una figura en ASCII).

Resultado esperado
El servidor mostrará en consola que recibió la petición.

El cliente imprimirá la figura recibida en la terminal.

NachOS finalizará con el mensaje: Machine halting!

Notas:
Si el cliente muestra "Error al conectar", asegúrese de que el servidor esté corriendo y que no haya errores en la IP o el puerto.

Use Ctrl+C para detener el servidor cuando termine.

Resumen rápido

Terminal 1: Ejecutar servidor

```bash
make
./exec/ThreadMirrorServer.out
```

Terminal 2: Ejecutar cliente NachOS

```bash
cd code/userprog
./nachos -x ../test/nachitos
```
