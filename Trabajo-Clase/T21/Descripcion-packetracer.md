# Descripción del Proyecto Packet Tracer: Conexión entre Isla 5 e Isla 6

Este documento describe la topología y configuración del proyecto desarrollado en Cisco Packet Tracer, en el cual se conectan **Isla 5** y **Isla 6** utilizando un **Switch de Capa 3 (3560)** compartido. La configuración fue validada con pings exitosos entre todas las estaciones de trabajo.

---

## Objetivo

Conectar dos islas (VLANs distintas) mediante un switch de capa 3 para permitir comunicación entre estaciones de trabajo (PCs) ubicadas en distintas VLANs, asegurando la segmentación del tráfico y el enrutamiento entre VLANs.

---

## Equipamiento utilizado

- **1 Switch Capa 3 (3560)** – compartido entre las dos islas
- **2 Switches Capa 2 (2960)** – uno por isla
- **8 PCs en total** – 4 por isla
- **Conexiones por cable cobre directo (Copper Straight-through)** entre PCs y switches
- **Conexiones trunk entre switches L2 y L3**

---

## VLANs y Direccionamiento

### Isla 5 (VLAN 350)
- VLAN ID: **350**
- Subred: `172.16.123.80/28`
- Gateway: `172.16.123.81` (SVI en Switch L3)
- IPs asignadas a PCs: `172.16.123.83` – `172.16.123.86`
- IP del switch L2: `172.16.123.82`

### Isla 6 (VLAN 360)
- VLAN ID: **360**
- Subred: `172.16.123.96/28`
- Gateway: `172.16.123.97` (SVI en Switch L3)
- IPs asignadas a PCs: `172.16.123.99` – `172.16.123.102`
- IP del switch L2: `172.16.123.98`

---

## Conexiones físicas

- Cada grupo de 4 PCs está conectado a puertos Fa0/13 – Fa0/16 en su respectivo Switch L2.
- Los switches L2 se conectan al Switch L3 por un enlace troncal:
  - Isla 5: Fa0/24 ↔ Fa0/5
  - Isla 6: Fa0/24 ↔ Fa0/6

---

## Configuración Importante

### En el Switch Capa 3 (3560)

- `ip routing` habilitado
- Interfaz VLAN 350: IP `172.16.123.81`
- Interfaz VLAN 360: IP `172.16.123.97`
- Puertos Fa0/5 y Fa0/6 configurados como trunk (`dot1q`)

### En los Switches Capa 2 (2960)

- Creación de la VLAN respectiva (350 o 360)
- Asignación de puertos de acceso a la VLAN
- Fa0/24 configurado como trunk hacia el Switch L3
- IP de gestión en la VLAN correspondiente

---

## Verificación de conectividad

Se realizaron pings desde PCs de Isla 5 a Isla 6 y viceversa. Todos los resultados fueron exitosos, confirmando:

- Comunicación interna en cada VLAN
- Enrutamiento correcto entre VLANs
- Comunicación con switches de gestión y gateway

---

## Archivo .pkt

El archivo Packet Tracer contiene:

- Dispositivos nombrados por isla
- VLANs correctamente etiquetadas
- IPs y conexiones validadas con pruebas de `ping`

---

## Conclusión

La red cumple con los objetivos propuestos: segmentación por VLAN, conexión entre islas por enrutamiento en capa 3, y correcta asignación de IPs. Esta topología es la que está para implementarse en el entorno físico.

