# Manual de Uso — Servidor y Cliente SSL (IPv6 + IPv4)

Este sistema implementa un **cliente y servidor SSL/TLS** que permite el intercambio seguro de mensajes mediante sockets, compatible tanto con **IPv6** como con **IPv4**. A continuación, se detalla el procedimiento completo para compilar, generar certificados y ejecutar ambos programas.

---

## Requisitos Previos

- Sistema Linux (probado en Fedora y similares).
- Bibliotecas de OpenSSL instaladas:  

  ```bash
  sudo dnf install openssl openssl-devel
  ```

Compilador g++ y utilidades de desarrollo (make, etc).

## Generar Certificado SSL

Ambos programas requieren un certificado ci0123.pem para funcionar correctamente. Para generarlo:

 ```bash
openssl req -x509 -nodes -days 365 -newkey rsa:2048 -keyout ci0123.pem -out ci0123.pem
 ```

Durante la creación se pedirá llenar algunos campos (la mayoría de ellos son opcionales, pero el common name preferiblemente localhost).

**Este archivo debe estar en el mismo directorio desde donde ejecute el servidor y el cliente, o bien especificar la ruta correspondiente en el código si decide moverlo.**

## Compilación

Desde la raíz del proyecto, ejecuta:

 ```bash
make all
 ```

Esto generará los ejecutables:

-exec/SSLServer.out

-exec/SSLClient.out

## Ejecución del Servidor

En una terminal:

 ```bash
./exec/SSLServer.out 4321
 ```

El servidor escucha en todas las interfaces disponibles (:: para IPv6 y compatibilidad con IPv4).

El número de puerto puede ser cambiado si se desea.

## Ejecución del Cliente

En otra terminal:

 ```bash
./exec/SSLClient.out <dirección> <puerto>
 ```

Ejemplos:

Para conexión local IPv6:

 ```bash
./exec/SSLClient.out ::1 4321
 ```

Para conexión local IPv4:

 ```bash
./exec/SSLClient.out 127.0.0.1 4321
 ```

Para conexión a un hostname (si tiene IPv6 o IPv4 en DNS):

 ```bash
./exec/SSLClient.out mi-servidor.example.com 4321
 ```

## Flujo del Programa

-Cliente
1.Inicializa contexto SSL.

2.Establece conexión TCP con el servidor.

3.Realiza el handshake SSL.

4.Solicita nombre de usuario y contraseña al usuario.

5.Envía un mensaje XML cifrado al servidor.

6.Recibe y muestra respuesta del servidor.

-Servidor
1.Inicializa contexto SSL y carga certificado.

2.Espera conexiones entrantes (IPv6 y IPv4).

3.Por cada cliente, realiza el handshake SSL.

4.Recibe credenciales XML.

5.Valida el mensaje:

Si coincide con:

 ```bash
<Body>
  <UserName>piro</UserName>
  <Password>ci0123</Password>
</Body>
 ```

responde con información del curso.

Si no, responde con "Invalid Message".

Cierra la sesión segura SSL.

## Limpieza

Para limpiar los archivos objeto y ejecutables generados:

 ```bash
make clean
 ```

## Observaciones

El código ya permite conexiones IPv6 y IPv4 (dual-stack) gracias al uso de AF_INET6 y setsockopt(IPV6_V6ONLY, 0).
