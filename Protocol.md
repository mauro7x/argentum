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
| 0 | Respuesta | S -> C | `OPCODE_REPLY (1) + LENGTH (4) + REPLY (LENGTH)` |
| 1 | Mensaje privado | S -> C | `SENDER_LENGTH (4) + SENDER (SENDER_LENGTH) + MSG_LENGTH (4) + MSG (MSG_LENGTH)` |
| 2 | Broadcast | S -> C | `TYPE(1)` |
| 128 | Comando | C -> S | `OPCODE_CMD (1) + ...` |
| 129 | Sign-in | C -> S | `USER_LENGTH (4) + USERNAME (USER_LENGTH) + PASS_LENGTH (4) + PASSWORD (PASS_LENGTH)` |
| 130 | Sign-up | C -> S | `USER_LENGTH (4) + USERNAME (USER_LENGTH) + PASS_LENGTH (4) + PASSWORD (PASS_LENGTH) + RACE (4) + KIND (4)` |

## PROPUESTA (MAU 24/06)

| OPCODE | TIPO | DIRECCIÓN | ESTRUCTURA (B) |
|--------|------|-----------|----------------|
| 0 | Mensaje | S -> C | `TYPE (1) + ...` |
| 1 | Broadcast | S -> C | `TYPE (1) + ...` |
| 128 | Comando | C -> S | `TYPE (1) + ...` |
| 129 | Sign-in | C -> S | `USER_LENGTH (4) + USERNAME (USER_LENGTH) + PASS_LENGTH (4) + PASSWORD (PASS_LENGTH)` |
| 130 | Sign-up | C -> S | `USER_LENGTH (4) + USERNAME (USER_LENGTH) + PASS_LENGTH (4) + PASSWORD (PASS_LENGTH) + RACE (4) + KIND (4) + HEAD_ID (4) + BODY_ID (4)` |

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

### Broadcast:
    
| TIPO | DESCRIPCION | ESTRUCTURA (B) |
|--------|------|----------------|
| 0 | NEW | `ENTITY_TYPE(1) + LENGTH (4) + BROADCAST (LENGTH)` | 
| 1 | UPDATE | `ENTITY_TYPE(1) + LENGTH (4) + BROADCAST (LENGTH)` | 
| 2 | DELETE | `ENTITY_TYPE(1) + ID (4)` | 

---

### Comandos (OP = 0)

| OPCODE | TIPO | ESTRUCTURA (B) |
|--------|------|----------------|
| 0 | Empezar a mover arriba | `-` | 
| 1 | Empezar a mover abajo | `-` | 
| 2 | Empezar a mover a la izquierda | `-` | 
| 3 | Empezar a mover a la derecha | `-` | 
| 4 | Dejar de mover | `-` |
| 5 | Usar arma principal | `POS` |
| 6 | Equipar objeto | `POSICION INVENTARIO (SLOT) (1)` |
| 7 | Tomar | `-` |
| 8 | Tirar | `POSICION INVENTARIO (SLOT) (1)` |
| 9 | Tirar | `POSICION INVENTARIO (SLOT) (1) + CANTIDAD (4)` |

| 10 | Meditar | `-` |
| 11 | Resucitar | `-` |

| 12 | Resucitar | `POS` |
| 13 | Curar | `POS` |

| 14 | Listar | `POS` |
| 15 | Depositar | `POS + POSICION INVENTARIO (SLOT) (1)` |
| 16 | Depositar | `POS + POSICION INVENTARIO (SLOT) (1) + CANTIDAD (4)` |
| 17 | Depositar oro | `POS + CANTIDAD (4)` |
| 18 | Retirar | `POS + ID (4)` |
| 19 | Retirar | `POS + ID (4) + CANTIDAD (4)` |
| 20 | Retirar oro | `POS + CANTIDAD (4)` |
| 21 | Comprar | `POS + ID (4)` |
| 22 | Comprar | `POS + ID (4) + CANTIDAD (4)` |
| 23 | Vender | `POS + ID (4)` |
| 24 | Vender | `POS + ID (4) + CANTIDAD (4)` |

| 25 | Listar jugadores conectados | `` |
| 26 | Mensaje | `NICK_LENGTH (4) + NICKNAME (NICK_LENGTH)` |

* `POS` = `X (4) + Y (4)`

---

## Respuestas (OP = 1)

| OPCODE | TIPO |
|--------|------|
| 0 | Error (rojo) |
| 1 | Informacion (blanco) |
| 2 | Éxito (verde) |
| 3 | Lista (amarillo) |

---