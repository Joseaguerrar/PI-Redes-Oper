Semana 3

Objetivos
   Completar la funcionalidad de la clase SSLSocket 

   🧪 Prueba de comunicación UDP sobre IPv6
Este incluye una prueba de comunicación cliente-servidor utilizando el protocolo UDP sobre IPv6. Ya que es la única forma de comunicarse desde fuera de la red interna de la ECCI para IPv6. 

📦 Archivos involucrados
tcp-cli.cc: Cliente UDP IPv6 (a pesar del nombre, no utiliza TCP)

ipv6-udp-server.cc: Servidor UDP IPv6 que espera un mensaje y responde con un saludo

⚠ Importante: No se utiliza SSL
Esta prueba no utiliza SSL ya que el servidor está basado en UDP, y OpenSSL está orientado a conexiones TCP.

Por lo tanto, no se debe usar SSLSocket para esta prueba.

✅ Requisitos para ejecutar la prueba
Ejecutar ambos programas en la misma máquina o en máquinas dentro de la misma red.

Estar dentro de la red de la ECCI o conectado por VPN, ya que se usa una dirección IPv6 link-local.

Tener habilitado IPv6 en la interfaz de red.

🧭 Pasos para realizar la prueba
Verificar interfaz de red activa

Ejecutar:

ip -6 addr
Buscar la dirección que comienza con fe80:: y anotar el nombre de la interfaz (ej: eth0, enp0s31f6, wlp3s0).

Editar el cliente

En tcp-cli.cc, asegurarse de que las siguientes líneas estén correctamente configuradas:

const char * whalev6 = "fe80::<DIRECCIÓN>";  // sin el %
const char * interface = "<INTERFAZ>";       // nombre detectado con ip -6 addr
Por ejemplo:

const char * whalev6 = "fe80::215:5dff:feb9:6c72";
const char * interface = "eth0";
Compilar el proyecto

make clean
make
Ejecutar el servidor en una terminal

make run-server
El servidor quedará esperando mensajes.

Ejecutar el cliente en otra terminal

make udp-run
✅ Resultado esperado
En la terminal del servidor:

Server: message received: Mensaje desde el cliente UDP
Server: Hello message sent.
En la terminal del cliente:

Mensaje enviado al servidor.
Respuesta del servidor: Hello from CI0123 server

Para ejecutar la prueba debe mover ambos el servidor y cliente a la carpeta de T3 en general para que funcione con el makefile.