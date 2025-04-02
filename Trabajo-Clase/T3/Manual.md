# Manual de Compilación

## 📦 Requisitos

Antes de compilar este proyecto, asegurate de tener instaladas las siguientes herramientas y librerías:

- `g++` (compilador de C++)
- `gcc` (compilador de C)
- `make` (herramienta de automatización)
- OpenSSL y sus encabezados de desarrollo:
  - En Fedora/RHEL:
    ```bash
    sudo dnf install openssl-devel
    ```
  - En Debian/Ubuntu:
    ```bash
    sudo apt install libssl-dev
    ```

## 🛠️ Compilación del proyecto

Para compilar todos los ejecutables necesarios:

```bash
make

```

Esto generará:

Archivos objeto (.o) en la carpeta obj/

Ejecutables en la carpeta exec/:

ipv4-ssl-cli.out → Cliente orientado a objetos (Socket/SSLSocket)

ssl-cli.out → Cliente procedimental SSL

▶️ Comandos de ejecución
make run
Ejecuta el cliente orientado a objetos (ipv4-ssl-cli.out) con una conexión segura SSL/TLS usando la clase SSLSocket.

Usa el puerto 443

Realiza un GET a una URL sobre HTTPS

Ejemplo interno:

```cpp
client = new SSLSocket();
```

make run-no-ssl

Ejecuta el mismo cliente (ipv4-ssl-cli.out) pero usando una conexión TCP sin cifrado con la clase Socket.

Usa el puerto 80

Realiza el mismo GET pero por HTTP (sin SSL)

make ssl-run

Ejecuta el cliente procedimental (ssl-cli.c) con SSL.

Usa una dirección IP y puerto por defecto (hardcoded)

Conexión SSL directa usando OpenSSL (SSL_connect, etc.)

Sin clases ni herencia

make ssl-run-args
Ejecuta el cliente ssl-cli.c con argumentos personalizados.

Usa argv[1] como host y argv[2] como puerto o nombre de servicio

Ejemplo:

```bash
./exec/ssl-cli.out redes.ecci.ucr.ac.cr https
```

🧹 Limpieza

Para eliminar los archivos generados por la compilación:

```bash
make clean
```

Esto elimina:

Todos los archivos .o en obj/

Todos los ejecutables .out en exec/