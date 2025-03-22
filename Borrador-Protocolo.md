# Protocolo Cliente-Servidor para Envío de Figuras ASCII (borrador)

## 1. Objetivo

Definir cómo un cliente puede ver y pedir figuras ASCII a través de un servidor principal que actúa como intermediario. Las figuras están distribuidas en varios servidores secundarios. El servidor principal hace un `fork()` para delegar la búsqueda a un proceso hijo, que consulta a los servidores secundarios **uno por uno**.

---

## 2. Componentes

- **Cliente:** Pide ver figuras (`LIST`) o solicita una en particular (`GET <nombre>`).
- **Servidor principal:** Recibe la solicitud del cliente. Si es `LIST`, responde directamente. Si es `GET`, hace `fork()` y el hijo se encarga de buscar en los servidores secundarios.
- **Servidor intermediario (el hijo):** Proceso hijo del servidor principal que revisa secuencialmente qué servidor secundario tiene la figura.
- **Servidores secundarios:** Guardan las figuras y pueden responder si tienen o no la solicitada.

---

## 3. Flujo de comunicación

### 3.1 Cliente quiere ver figuras

- Cliente manda:
LIST


- Servidor principal responde algo como:
OK

GATO

PERRO

CASA

BARCO

*Puede estar precargado o consolidado al inicio.*

---

### 3.2 Cliente pide una figura

1. Cliente manda:
GET GATO

2. Servidor principal hace `fork()`.  
- El hijo se convierte en un "buscador de figuras".

3. El proceso hijo va preguntando a cada servidor secundario, **uno por uno** (sin concurrencia momentaneamente).

Ejemplo del flujo interno:
Preguntar al servidor A → NOTFOUND
Preguntar al servidor B → FOUND
→ devolver la figura al servidor principal

4. El servidor principal recibe la respuesta del hijo y responde al cliente:

- Si la encontró:
  ```
  OK
  <figura ASCII>
  ```

- Si no:
  ```
  ERROR
  Figura no encontrada
  ```
- Si escribió mal la solicitud:
  ```
  ERROR
  Error en la solicitud
  ```
---

## 4. Respuestas esperadas entre procesos

### Entre proceso hijo y servidores secundarios:

- Si el servidor **tiene** la figura:
FOUND <figura>

- Si **no la tiene**:
NOTFOUND

### Entre proceso hijo y servidor principal (vía pipe, archivo, etc.):

- Envía lo que encontró o una señal de error.

---

## 5. Ejemplo

**Cliente:**
GET BARCO

**Servidor principal crea proceso hijo**

**Hijo:**
Consulta a servidor1 → NOTFOUND
Consulta a servidor2 → FOUND

**Servidor principal:**
OK


                  ~.
           Ya...___|__..aab     .   .
            Y88a  Y88o  Y88a   (     )
             Y88b  Y88b  Y88b   `.oo'
             :888  :888  :888  ( (`-'
    .---.    d88P  d88P  d88P   `.`.
   / .-._)  d8P'"""|"""'-Y8P      `.`.
  ( (`._) .-.  .-. |.-.  .-.  .-.   ) )
   \ `---( O )( O )( O )( O )( O )-' /
    `.    `-'  `-'  `-'  `-'  `-'  .' CJ
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

---

## 6. Observaciones

- Por ahora no hay concurrencia.
- Solo un `fork()` y búsqueda secuencial.
- Se puede ampliar luego para hacer búsquedas paralelas o más eficientes.