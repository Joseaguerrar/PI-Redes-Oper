# 🗓️ Semana 3

## 🎯 Objetivos
Completar la funcionalidad de la clase `SSLSocket` y realizar una **prueba de comunicación cliente-servidor sobre TCP con SSL utilizando IPv6**.

---

## 🔐 Prueba: Comunicación TCP/SSL sobre IPv6

Esta prueba consiste en una comunicación segura entre cliente y servidor utilizando el protocolo **TCP** sobre **IPv6**, utilizando **SSL/TLS** para cifrado.  
Se usa una dirección **IPv6 link-local**, por lo tanto, esta prueba **solo funcionará** si estás:

- Dentro de la red interna de la **ECCI**, o  
- Conectado a través de la **VPN** de la ECCI/UCR.

---

## 📦 Archivos involucrados

- `tcp-cli.cc`: Cliente TCP/SSL IPv6  
- `ipv6-ssl-server.cc`: Servidor TCP/SSL IPv6 que espera un mensaje y responde con un saludo  
- `SSLSocket.{h,cpp}`: Implementación de socket seguro usando OpenSSL  
- `VSocket.{h,cpp}` y `Socket.{h,cpp}`: Clases base y derivada para manejo de sockets  

---

## ⚠ Importante

- Esta prueba **sí utiliza SSL**, por lo tanto, debe usarse la clase `SSLSocket` tanto en el cliente como en el servidor.
- La dirección IPv6 usada es **link-local**, lo que implica que se debe indicar también la **interfaz de red** utilizada (ej: `eth0`, `enp0s31f6`, `wlp3s0`, etc.).
- No es posible ejecutar esta prueba desde fuera de la red de la UCR sin VPN.

> 🔒 Nota: En esta versión no se utilizan archivos de certificado (`cert.pem`, `key.pem`). La conexión SSL se establece sin autenticación del servidor, lo cual es aceptable para fines de prueba.

---

## ✅ Requisitos para ejecutar la prueba

- Tener habilitado **IPv6** en la interfaz de red.
- Ejecutar ambos programas en la **misma máquina** o en **máquinas dentro de la misma red**.
- Estar **dentro de la red de la ECCI** o **conectado por VPN**.

---

## 🧭 Pasos para realizar la prueba

### 1. Verificar interfaz de red activa

```bash
ip -6 addr
```

Buscar la dirección que comienza con fe80:: y anotar el nombre de la interfaz (ej: eth0, enp0s31f6, wlp3s0).

2. Ejecutar el cliente
El cliente permite pasar la interfaz como argumento:

```bash
./exec/ipv6-ssl-cli.out enp0s31f6
```

Este valor será usado internamente para establecer el scope_id necesario para conectar a una dirección fe80::.

3. Compilar el proyecto

```bash
make clean
make
```

4. Ejecutar el servidor en una terminal

```bash
make run-server
```

Esto iniciará el servidor SSL a la espera de conexiones entrantes.

5. Ejecutar el cliente en otra terminal

```bash
make run-client
```

✅ Resultado esperado
En la terminal del servidor:

```bash
Esperando conexiones SSL en IPv6...
Mensaje del cliente: Mensaje desde el cliente TCP
```

En la terminal del cliente:

```bash
Mensaje enviado al servidor SSL.
Respuesta del servidor: Respuesta del servidor SSL
```

📁 Organización
Para ejecutar la prueba correctamente usando el Makefile, ambos archivos (tcp-cli.cc y ipv6-ssl-server.cc) deben estar dentro de la carpeta T3 o donde esté ubicado el Makefile.

```mathematica
T3/
├── Makefile
├── tcp-cli.cc
├── ipv6-ssl-server.cc
├── VSocket.{h,cpp}
├── Socket.{h,cpp}
└── SSLSocket.{h,cpp}
```
