# Guía para Compilar y Ejecutar Programas de Usuario en NachOS (64 bits)

## Instalación del entorno NachOS en Linux

### Requisitos generales

Necesitarás:

- `make`
- `gcc` (preferiblemente versión que soporte MIPS cross-compilation)
- Bibliotecas de 32 bits si estás en un sistema de 64 bits

---

### Fedora (y derivados como RHEL/CentOS)

```bash
sudo dnf groupinstall "Development Tools"
sudo dnf install glibc-devel.i686 g++-multilib ncurses-devel.i686
```

Ubuntu/Debian (y derivados como Linux Mint)

```bash
sudo apt update
sudo apt install build-essential gcc-multilib g++-multilib
```

Arch Linux / Manjaro

```bash
sudo pacman -S base-devel gcc-multilib
```

## Compilar programas de usuario de NachOS

1. Crear el programa coff2noff
NachOS utiliza un formato propio de ejecutables llamado NOFF, que se genera a partir de ejecutables en formato COFF. Para convertir de COFF a NOFF, debes compilar el programa auxiliar coff2noff.

En el directorio bin de NachOS:

```bash
cd code/bin
make coff2noff
```

2. Crear un programa de usuario
En el directorio test:

```bash
cd ../test
nano nachitos.c #si quiere, si no lo crea normal
Escriba su programa usando llamadas al sistema como Socket, Write, Read, Exit, etc.
guarda con Ctrl+O y sale de nano con Ctrl+X
```

3. Compilar el programa

```bash
make nachitos
```

Esto realiza:

mips-gcc compila nachitos.c → nachitos.o

mips-ld enlaza con start.o → nachitos.coff

coff2noff convierte a formato ejecutable NachOS → nachitos

4. Dar permisos de ejecución (opcional)

```bash
chmod +x nachitos
```

5. Verificar que el archivo se generó

```bash
ls
```

Debe aparecer el archivo nachitos.
6. Ejecutar el programa
Desde userprog:

```bash
cd ../userprog
./nachos -x ../test/nachitos
```

Esto ejecuta el archivo nachitos en el simulador NachOS.

Resumen rápido

Requisitos (Fedora):

```bash
sudo dnf install glibc-devel.i686
```

En bin:

```bash
cd code/bin
make coff2noff
```

En test:

```bash
cd ../test
nano nachitos.c        # Crear programa
make nachitos          # Compilar
ls                     # Verificar que se creó
```

En userprog:

```bash
cd ../userprog
./nachos -x ../test/nachitos
```
