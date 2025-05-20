# ¿Cómo es leído el encabezado del programa ejecutable NachOS?

El encabezado es leído por el constructor de AddrSpace, específicamente mediante una llamada al método ReadAt del objeto OpenFile. Este método carga en memoria una estructura llamada NoffHeader desde el inicio del archivo ejecutable.

executable->ReadAt((char *)&noffH, sizeof(noffH), 0);

## ¿De qué tamaño es el encabezado?

El encabezado (NoffHeader) tiene un tamaño de 32 bytes.

Esto incluye información sobre:

El número mágico (magic)

El segmento de texto (code)

El segmento de datos inicializados (initData)

El segmento de datos no inicializados (uninitData)

## ¿Cuál es el número mágico?

El número mágico (magic number) es:
0x00badfad
Este valor se utiliza para verificar que el archivo tiene el formato esperado (NOFF). Si no coincide, NachOS no carga el ejecutable.

## Verificación con hexdump

hexdump -n 128 -C ../test/halt
Salida relevante:

00000000  ad df ba 00 ...
Nota: 0x00badfad aparece en orden little endian como ad df ba 00.

## Análisis de la estructura de segmentos (NOFF) del ejecutable

La estructura NoffHeader define los siguientes segmentos:

code → Segmento de texto (instrucciones)

initData → Segmento de datos inicializados

uninitData → Segmento de datos no inicializados (bss)

Cada segmento tiene:

virtualAddr: dirección virtual

inFileAddr: posición en el archivo

size: tamaño del segmento

## ¿Cómo es establecido el tamaño del proceso?

El tamaño del proceso se determina como la suma de los tamaños de:

code.size

initData.size

uninitData.size

Tamaño de la pila (UserStackSize)

Este total es redondeado hacia arriba al múltiplo más cercano del tamaño de página (división entera).

unsigned int size = noffH.code.size + noffH.initData.size + noffH.uninitData.size + UserStackSize;
numPages = divRoundUp(size, PageSize);

## ¿Cuál es el tamaño del segmento de código (T)?

Para halt, code.size = 40 bytes.

## ¿Cuál es el tamaño del segmento de datos inicializados (D)?

initData.size = 0 bytes.

¿Cuál es el tamaño de la pila (S)?
La pila se define con una constante:

**#define UserStackSize 1024 // en bytes**
Es decir, el tamaño de la pila es 1024 bytes o 8 páginas (si PageSize es 128).

## ¿Cuál es el tamaño de una página?

El tamaño de una página en NachOS es de:
 **#define PageSize 128 // en bytes**

## ¿Alguno de estos tamaños puede ser cero?

Sí. En el caso de halt:

initData.size = 0

uninitData.size = 0

Esto es común en programas muy pequeños o simples.

## ¿Cómo el programa es cargado (del disco) a memoria?

Cada segmento (code y initData) se carga desde el archivo ejecutable a memoria usando ReadAt, indicando:

La dirección destino (memoria)

Cuántos bytes leer

Desde qué posición del archivo

executable->ReadAt(&mainMemory[addr], size, inFileAddr);

## ¿Cómo se realiza la lectura del disco?

Usando el método ReadAt de OpenFile, que accede al disco virtual (simulado) de NachOS.

## ¿Cómo se determina el archivo de donde se realiza la lectura?

Es el archivo pasado como parámetro al constructor de AddrSpace:

OpenFile *executable = fileSystem->Open(filename);

## ¿Cómo se indica la cantidad de bytes que se quiere leer?

El segundo parámetro de ReadAt indica cuántos bytes leer:

ReadAt(char *into, int numBytes, int position);

## ¿Cómo hacer para leer del archivo solo una página (128 bytes)?

Solo debes pasar 128 como segundo parámetro:

executable->ReadAt(&mainMemory[destination], 128, fileOffset);

## ¿Cuál es la posición dentro del archivo de lectura?

Se obtiene del campo inFileAddr del segmento correspondiente (code o data):

noffH.code.inFileAddr
noffH.initData.inFileAddr

## ¿Cómo es colocada en la memoria la información leída del disco?

Usando la dirección virtual (virtualAddr) como índice en mainMemory, pero primero traducida a física usando la tabla de páginas (pageTable).

## ¿Cuál es la posición en la memoria?

La posición final depende de:

virtualAddress → página lógica → página física (usando pageTable) → posición real en mainMemory

physicalAddr = pageTable[virtualPage].physicalPage * PageSize + offset;

## ¿Cómo se puede cambiar esta dirección?

Se modifica la tabla de páginas (TranslationEntry[] pageTable) para redirigir la página virtual a una página física distinta.

## Estrategia para colocar las páginas (128 bytes)

Leer de a 128 bytes desde code y data.

Traducir la dirección virtual de destino a física (usando pageTable).

Copiar cada fragmento en mainMemory.

## ¿Cómo manejar segmentos menores de 128 bytes?

Se puede leer 128 bytes igual, pero solo copiar los bytes útiles.

O leer solo los bytes necesarios (min(size, 128)).

Rellenar el resto con ceros si es necesario.

## ¿Cómo manejar páginas con contenido de varios segmentos?

Ejemplo: si una página contiene el final del código y el inicio de los datos.

Estrategia:

Leer la página entera desde el archivo (si es contigua).

Cargar por partes según el segmento que corresponde.

Si no es posible leer como bloque, cargar manualmente:

Parte de código

Parte de datos

Usar máscaras o condiciones para evitar sobrescribir datos ya colocados.

# 1. Clase TranslationEntry

La clase TranslationEntry representa una entrada en la tabla de páginas. Cada entrada describe cómo una página virtual de un proceso se traduce a una página física en la memoria.

class TranslationEntry {
  public:
    int virtualPage;    // número de página virtual
    int physicalPage;   // número de página física asignada
    bool valid;         // si la traducción es válida
    bool use;           // usado por el algoritmo de reemplazo (ej. LRU)
    bool dirty;         // si ha sido modificada
    bool readOnly;      // si es solo lectura
};

## 2. Traducción de direcciones: Machine::Translate()

Ubicación: machine/translate.cc

Este método se llama cada vez que se accede a memoria (lectura o escritura). Traduce una dirección virtual a una dirección física.

Lógica básica:
Divide la dirección virtual en: virtualPage y offset.

Usa pageTable[virtualPage] para obtener la physicalPage.

Calcula la dirección física:

physicalAddress = entry.physicalPage * PageSize + offset;
Si valid == false, lanza una page fault (interrupción).

## 3. Machine::ReadMem() y WriteMem()

Ambos usan Translate() para obtener la dirección física y luego acceden a mainMemory.

Ejemplo de lectura:

bool success = Translate(virtAddr, &physAddr, size, false);
data = mainMemory[physAddr];

## 4. Estructura de la máquina MIPS (machine.h)

class Machine {
  public:
    TranslationEntry *pageTable;  // Tabla de páginas del proceso actual
    unsigned int pageTableSize;

    char *mainMemory;             // Memoria física real
    ...
};

## 5. AddrSpace::InitRegisters()

Inicializa los registros antes de correr el proceso:

void AddrSpace::InitRegisters() {
    for (int i = 0; i < NumTotalRegs; i++)
        machine->WriteRegister(i, 0);

    machine->WriteRegister(PCReg, 0);  // Inicio del código
    machine->WriteRegister(NextPCReg, 4);
    machine->WriteRegister(StackReg, numPages * PageSize - 16);
}

## 6. AddrSpace::RestoreState() y SaveState()

RestoreState()
Asigna la tabla de páginas del proceso actual:

void AddrSpace::RestoreState() {
    machine->pageTable = pageTable;
    machine->pageTableSize = numPages;
}
SaveState()
Vacío en muchas implementaciones simples, o guarda temporalmente el estado del proceso saliente.

## 7. ¿Cómo se llena pageTable en el constructor de AddrSpace?

En el constructor se asignan las páginas virtuales a páginas físicas así:

pageTable[i].virtualPage = i;
pageTable[i].physicalPage = i;  // (0 → 0, 1 → 1, ...)
pageTable[i].valid = TRUE;
pageTable[i].use = FALSE;
pageTable[i].dirty = FALSE;
pageTable[i].readOnly = FALSE;
Este esquema no permite múltiples procesos simultáneos, porque todos usarían las mismas páginas físicas.

## 8. ¿Cómo cambiar esto? → Usar un BitMap

Objetivo:
Simular la memoria física con un mapa de bits para saber qué páginas están libres u ocupadas.

## 9. Crear BitMap global

En system.h:
**# ifdef USER_PROGRAM**
**# include "bitmap.h"**
**extern BitMap *MiMapa;  // Variable global para el mapa de memoria**
**extern Lock*MiMapaLock; // Acceso exclusivo si hay hilos concurrentes**
**# endif**

En system.cc:
**# ifdef USER_PROGRAM**
**BitMap *MiMapa;**
**Lock*MiMapaLock;**
**# endif**

En Initialize() de system.cc:
**# ifdef USER_PROGRAM**
    **MiMapa = new BitMap(NumPhysPages);       // Cada bit representa una página física**
    **MiMapaLock = new Lock("MiMapaLock");**
**# endif**
**Usa NumPhysPages, definido en machine.h. Usualmente es 32 o 128.**

## 10. Asignación segura con concurrencia

En el constructor de AddrSpace, en vez de pageTable[i].physicalPage = i, hacer:

**MiMapaLock->Acquire();**
**int freePage = MiMapa->Find();  // Devuelve un índice libre**
**MiMapaLock->Release();**

**if (freePage == -1) {**
    **// Error: no hay espacio disponible**
**}**
***pageTable[i].physicalPage = freePage**

## 11. Liberar páginas al final del proceso

En Exit, debes devolver las páginas al mapa de bits:

for (int i = 0; i < numPages; ++i) {
    MiMapaLock->Acquire();
    MiMapa->Clear(pageTable[i].physicalPage);
    MiMapaLock->Release();
}

## 12. Cómo se carga el programa a memoria en NachOS?

NachOS carga un ejecutable en memoria en el constructor de AddrSpace. Se realiza lo siguiente:

**Se abre el archivo binario.**

**Se lee el encabezado (NOFF).**

**Se determina el tamaño total del programa (code + initData + uninitData + stack).**

**Se asignan páginas físicas mediante el BitMap.**

**Finalmente, el archivo se lee en memoria usando ReadAt.**

## 13. Qué hace el método ReadAt de la clase OpenFile?

El método ReadAt permite leer una cantidad específica de bytes desde una posición dada del archivo:

**int OpenFile::ReadAt(char *into, int numBytes, int position)**

Parámetros:
**char*into: el buffer donde se almacenan los datos leídos.**

**int numBytes: cuántos bytes se desean leer.**

**int position: desde qué posición del archivo comenzar a leer.**

## 14. Por qué hay dos llamados a ReadAt?

Porque hay al menos dos segmentos que se deben cargar desde el archivo a memoria:

*Uno para el segmento de código (code).

*Otro para el segmento de datos inicializados (initData).

Cada segmento tiene diferente posición y tamaño, por lo tanto se necesitan lecturas independientes.

## 15. Estructura del archivo ejecutable NachOS (NOFF)

|------------------|
| Encabezado (H)   |  (noffHeader)
|------------------|
| Texto (TX)       |  Código (instrucciones ejecutables)
|------------------|
| Datos Init. (DI) |  Variables globales con valor
|------------------|
| Datos No Init.   |  Variables globales sin valor (se reservan)
|------------------|
| Pila (S)         |  Stack del programa
|------------------|

## 16. Qué es el número mágico?

El número mágico (magic number) en NOFF es 0xBADFAD (aunque puede variar en endianess). Se usa para identificar si el archivo tiene un formato válido. Se valida al inicio del constructor de AddrSpace.

## 17. Cómo se leen los segmentos por páginas (128 bytes)?

for (int i = 0; i < code.size; i += PageSize) {
   int bytesToRead = min(PageSize, code.size - i);
   int physAddr = pageTable[virtPage].physicalPage * PageSize;
   executable->ReadAt(&mainMemory[physAddr], bytesToRead, code.inFileAddr + i);
}
**Se hace lo mismo para initData**

## 18. Qué pasa si el segmento no es múltiplo de 128 bytes?

El último ReadAt se hace con menos de 128 bytes (min(PageSize, size - i)), para evitar sobrelectura. Solo se lee lo necesario.

## 19. Qué se hace con la pila y los datos no inicializados?

Aunque no se leen del archivo, se reservan páginas en memoria mediante BitMap y se agregan a pageTable. Se inicializan a cero o se dejan sin tocar hasta su uso.

## 20. Qué se debe deshacer al terminar el programa (Exit)?

Liberar las páginas físicas en el BitMap (MiMapa->Clear()).

Destruir la AddrSpace.

Terminar el hilo con currentThread->Finish().
