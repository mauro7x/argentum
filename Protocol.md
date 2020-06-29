# Protocolo de comunicación

Hacer una descripción...

## Estructura

Cada mensaje que  .. .. . .

*Cuando se utilice ... significará que dependerá de cada caso particular.*

`OPCODE (1 byte) | ...`

Identificas el mensaje con el primer byte recibido (opcode), y para cada caso se prosigue distinto.

---

## Casos generales

| OPCODE | TIPO | DIRECCIÓN | ESTRUCTURA (B) |
|--------|------|-----------|----------------|
| 0 | Mensaje | S -> C | `TYPE (1) + ...` |
| 1 | Broadcast | S -> C | `TYPE (1) + ...` |
| 128 | Comando | C -> S | `TYPE (1) + ...` |
| 129 | Sign-in | C -> S | `USER_LENGTH (4) + USERNAME (USER_LENGTH) + PASS_LENGTH (4) + PASSWORD (PASS_LENGTH)` |
| 130 | Sign-up | C -> S | `USER_LENGTH (4) + USERNAME (USER_LENGTH) + PASS_LENGTH (4) + PASSWORD (PASS_LENGTH) + RACE (4) + KIND (4) + HEAD_ID (4) + BODY_ID (4)` |

---

### Message (OP = 0):

| OPCODE | TIPO | ESTRUCTURA (B) |
|--------|------|----------------|
| 0 | Mensaje de error del server | `MSG_LENGTH (4) + MSG (MSG_LENGTH)` |
| 1 | Mensaje de información del server | `MSG_LENGTH (4) + MSG (MSG_LENGTH)` |
| 2 | Mensaje de éxito del server | `MSG_LENGTH (4) + MSG (MSG_LENGTH)` |
| 3 | Lista del server | `ver lista` |
| 128 | Mensaje privado | `SENDER_LENGTH (4) + SENDER (SENDER_LENGTH) + MSG_LENGTH (4) + MSG (MSG_LENGTH)` |
| 129 | Mensaje general | `SENDER_LENGTH (4) + SENDER (SENDER_LENGTH) + MSG_LENGTH (4) + MSG (MSG_LENGTH)` |

---

### Broadcast (OP = 1):
    
| TIPO | DESCRIPCION | ESTRUCTURA (B) |
|--------|------|----------------|
| 0 | NEW | `ENTITY_TYPE(1) + LENGTH (4) + BROADCAST (LENGTH)` | 
| 1 | UPDATE | `ENTITY_TYPE(1) + LENGTH (4) + BROADCAST (LENGTH)` | 
| 2 | DELETE | `ENTITY_TYPE(1) + ...` | 

#### Especialización para DELETE

| TIPO | DESCRIPCION | ESTRUCTURA (B) |
|--------|------|----------------|
| 0 | Player | Combinación no válida | 
| 1 | Character | `ID (4)` | 
| 2 | Creature | `ID (4)` |
| 3 | Item | `POS` |

---

### Comandos (OP = 128)

| OPCODE | TIPO | ESTRUCTURA (B) |
|--------|------|----------------|
| 0 | Empezar a mover arriba | `-` |
| 1 | Empezar a mover abajo | `-` |
| 2 | Empezar a mover a la izquierda | `-` |
| 3 | Empezar a mover a la derecha | `-` |
| 4 | Dejar de mover | `-` |
| 5 | Usar arma principal | `TARGET (INSTANCEID) (4)` |
| 6 | Equipar objeto | `SLOT INVENTARIO (1)` |
| 7 | Desequipar objeto | `POSICION EQUIPAMIENTO (SLOT) (1)` |
| 8 | Tomar | `-` |
| 9 | Tirar | `SLOT INVENTARIO (1) + CANTIDAD (4)` |
| 10 | Meditar | `-` |
| 11 | Resucitar | `-` |
| 12 | Listar comandos disponibles del NPC (/help) | `POS` |
| 13 | Resucitar | `POS` |
| 14 | Curar | `POS` |
| 15 | Listar | `POS` |
| 16 | Depositar | `POS + SLOT INVENTARIO (1) + CANTIDAD (4)` |
| 17 | Depositar oro | `POS + CANTIDAD (4)` |
| 18 | Retirar | `POS + ID (4) + CANTIDAD (4)` |
| 19 | Retirar oro | `POS + CANTIDAD (4)` |
| 20 | Comprar | `POS + ID (4) + CANTIDAD (4)` |
| 21 | Vender | `POS + SLOT INVENTARIO (1) + CANTIDAD (4)` |
| 22 | Listar jugadores conectados | `` |
| 23 | Mensaje general | `CONTENT_LENGTH (4) + CONTENT (LENGTH)` |
| 24 | Mensaje privado | `NICK_LENGTH (4) + NICKNAME (NICK_LENGTH) + CONTENT_LENGTH (4) + CONTENT (LENGTH)` |


* `POS` = `X (4) + Y (4)`

---
