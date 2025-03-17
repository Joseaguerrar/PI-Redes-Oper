# PI-Redes-Oper

Este proyecto implementa una comunicación cliente-servidor mediante sockets en **C++** utilizando **IPv4 e IPv6**.  
Incluye pruebas de conexión y transferencia de datos mediante un servidor HTTP simple en Python.

## 📁 **Estructura del Proyecto**
El proyecto contiene los siguientes archivos:
```bash
. ├── Makefile # Archivo para compilar y ejecutar los programas
  ├── server.py # Servidor HTTP en Python (IPv6) 
  ├── ipv4-test.cc # Cliente en C++ para pruebas con IPv4 
  ├── ipv6-test.cc # Cliente en C++ para pruebas con IPv6 
  ├── Socket.h # Interfaz de la clase Socket 
  ├── Socket.cc # Implementación de la clase Socket 
  ├── VSocket.h # Interfaz de la clase base VSocket 
  └── VSocket.cc # Implementación de la clase VSocket 
```
  

## 🛠 **Comandos disponibles en el Makefile**
Para compilar y ejecutar los programas, puedes usar los siguientes comandos:

### **🔹 Compilación**
```bash
make 4       # Compila ipv4-test.out
make 6       # Compila ipv6-test.out
make all     # Compila ambos ejecutables
make run4    # Compila y ejecuta ipv4-test.out
make run6    # Compila y ejecuta ipv6-test.out
make run NUM=4   # Ejecuta ipv4-test.out
make run NUM=6   # Ejecuta ipv6-test.out
make clean   # Elimina archivos objeto y ejecutables


Antes de ejecutar las pruebas en IPv6, es necesario iniciar el servidor en otra terminal en Python:


python3 server.py
```


### Respuestas esperadas
🔹 Respuesta esperada para IPv4 (make run4)

<HTML>
<HEAD>
<link rel="stylesheet" type="text/css" href="/aArt/css/aArt.css" media="screen">
</HEAD>
<BODY>
<DIV class="aArt">
<PRE>
              __ \ / __
             /  \ | /  \
                 \|/
            _,.---v---._
   /\__/\  /            \
   \_  _/ /              \
     \ \_|           @ __|
      \                \_
       \     ,__/       /
     ~~~`~~~~~~~~~~~~~~/~~~~
</PRE>
</DIV>
</HTML>

🔹 Respuesta esperada para IPv6 (make run6)

HTTP/1.0 200 OK
Server: SimpleHTTP/0.6 Python/3.10.12
Date: Mon, 17 Mar 2025 03:37:19 GMT
Content-type: text/html; charset=utf-8
Content-Length: 815 (variante)

🔍 Cómo determinar cuál curl usar
Si no estás seguro de cuál dirección usar en curl, ejecuta el siguiente comando:

```bash
ip -6 addr
```
Esto mostrará las direcciones IPv6 disponibles en tu máquina.
🔹 Busca una dirección fe80::... (Link-Local) asociada a tu interfaz de red (eth0, wlp2s0, etc.).
🔹 Si la dirección es fe80::215:5dff:feb9:60eb en la interfaz eth0, el comando correcto para probar la conexión con curl es:

```bash
curl [fe80::215:5dff:feb9:60eb%eth0]:8080
```
Si usas otra interfaz, reemplaza eth0 con el nombre de la interfaz correcta.

Si curl funciona correctamente, significa que el servidor está escuchando en IPv6 y make run6 debería devolver una respuesta similar.

🛠 Solución de problemas
Si make run6 no devuelve 200 OK:

Verificar que el servidor está corriendo
```bash
ps aux | grep python
```
Comprobar que el puerto 8080 está en uso por el servidor
```bash
netstat -tulnp | grep 8080
```
Revisar que la interfaz de red es correcta
```bash
ip -6 addr
```
