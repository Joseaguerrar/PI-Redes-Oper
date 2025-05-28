# Laboratorio Packet Tracer – Grupo 3, Equipo 5 – Isla 5

Este archivo Packet Tracer corresponde al desarrollo del laboratorio 3-5, configurando la red local del **Equipo 5 del Grupo 3**, asignado a la **Isla 5**. Se implementó una VLAN con servicio DHCP desde un switch de capa 3 (L3) y se verificó la conectividad entre los dispositivos.

---

## Objetivo

Configurar:

- Una VLAN (`VLAN 350`) para la isla asignada
- Un enlace trunk entre el Switch L2 (2960) y el Switch L3 (3560)
- Servicio DHCP desde el Switch L3
- Comunicación total entre las PCs y el gateway

---

## Topología y dispositivos utilizados

### Dispositivos

- 1 × Switch 2960 (L2)
- 1 × Switch 3560-24PS (L3)
- 4 × PCs

### Conexiones

| Dispositivo | Puerto         | Conecta con     | Puerto         |
|-------------|----------------|------------------|----------------|
| PC1         | Fa0/1          | Switch 2960      | Fa0/1          |
| PC2         | Fa0/2          | Switch 2960      | Fa0/2          |
| PC3         | Fa0/3          | Switch 2960      | Fa0/3          |
| PC4         | Fa0/4          | Switch 2960      | Fa0/4          |
| Switch 2960 | Gig0/1         | Switch 3560-24PS | Gig0/1         |

---

## Configuración

### Switch 2960 (L2)

```bash
configure terminal
vlan 350
name ISLA_5

interface range fa0/1 - 4
 switchport mode access
 switchport access vlan 350

interface gig0/1
 switchport mode trunk
 switchport nonegotiate
```

### Switch 3560 (L3)

```bash
configure terminal
vlan 350
name ISLA_5

interface vlan 350
 ip address 172.16.123.81 255.255.255.240
 no shutdown

interface gig0/1
 switchport trunk encapsulation dot1q
 switchport mode trunk
 switchport nonegotiate
 no shutdown

ip dhcp pool ISLA_5
 network 172.16.123.80 255.255.255.240
 default-router 172.16.123.81
 dns-server 8.8.8.8

ip routing
```

**PCs Configuradas en modo DHCP en Desktop > IP Configuration.**

## Verificaciones

### Estado de interfaces

```bash
show interfaces status
```

**Debería ver:**

```plaintext
Port      Name               Status       Vlan       Duplex  Speed Type
Fa0/1                        connected    350        a-full   a-100 10/100BaseTX
...
Gig0/1                       connected    trunk       a-full  a-1000 1000BaseX
```

```bash
show interfaces trunk
```

**Debería ver:**

```plaintext
Port        Mode         Encapsulation  Status        Native vlan
Gig0/1      on           802.1q         trunking      1

Port        Vlans allowed on trunk
Gig0/1      1-1005

Port        Vlans allowed and active in management domain
Gig0/1      1,350

Port        Vlans in spanning tree forwarding state and not pruned
Gig0/1      1,350
```

```bash
show ip interface brief
```

**Debería ver:**

```plaintext
Interface              IP-Address      OK? Method Status                Protocol
Vlan350                172.16.123.81   YES manual up                    up
GigabitEthernet0/1     unassigned      YES unset  up                    up

```

## Verificar que las PCs reciben IP por DHCP

En cada PC:

Ir a Desktop > IP Configuration

Seleccionar DHCP

Abrir Command Prompt y escribir:

```bash
ipconfig
```

Resultado esperado (ejemplo):

```bash
IP Address.........: 172.16.123.83
Subnet Mask........: 255.255.255.240
Default Gateway....: 172.16.123.81
```

## Comprobar conectividad con ping

**Desde una PC:**

```bash
ping 172.16.123.81  # Gateway
```

**Debería ver:**

```plaintext
Reply from 172.16.123.81: bytes=32 time<1ms TTL=255
```

**Desde una PC a otra:**

```bash
ping 172.16.123.84
```

**Debería ver:**

```plaintext
Reply from 172.16.123.84: bytes=32 time<1ms TTL=128
```

## Guardar configuración en switches L2 y L3

En cada switch:

```bash
write memory
```
