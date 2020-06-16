# Draft


## Clases

Server.
* Loop principal: escucha entrada hasta recibir una `q` y termina la ejecución, guardando el estado y avisando a los clientes.

GameLoop. MAIN GAME LOOP.
* Atributos:
    - Contenedor de jugadores conectados (PlayerConnections).
    - map<idjugador, playerconnection>

{
Accepter. ESTABLECE CONEXIÓN TCP.
* Atributos:
    - Contenedor de PlayerLogins. 
* Loop principal:
    - Hace accept **bloqueante** hasta tener una conexión válida (socket peer listo).
    - Instancia y lanza el PlayerLogin(socket peer).
    - Agrega el playerlogin al contenedor de players logins.
    - Recolector de basura de players logins.
}

PlayerLogin. OBJETO ACTIVO QUE SE ENCARGA DE METER (o no) AL JUGADOR AL JUEGO.
* Atributos:
    - Referencia al contenedor de jugadores activos (jugando).
* Loop principal:
    - Encapsular la conexion, hasta que se pueda conectar o que se vaya o timeout etc.
    - Caso en que el jugador se conecta: le dice al game que instancie la conexión.

PlayerConnection.
* Atributos:
    - Cola bloqueante de notificaciones a enviar.
    - Referencia a cola no bloqueante del MainGameLoop con comandos recibidos.
    - Lanza dos hilos, el sender (notification dispatcher) y el receiver (command receiver).


GameLoop.addPlayer(socket peer, PlayerData initial_data) {
    int id = Game.addCharacter(initial_data);
    PlayerConnection* new_player = new PlayerConnection(id, std::move(peer), coladecomandos);
    map.emplace(id, new_player);
}


Hacer que cuando el sender y el receiver de un playerconnection terminen, agreguen a la playerconnection a una cola de clientes que terminaron


---

Archivos en runtime:

DataBase database;
database.init();

* Base de datos:
[
    "nickname": pass,
    "nick2: pass2,
    ...
    "nickn": passn
]

map<nickname, pass> base_de_datos;
base_de_datos.persist()


---

# GameLoop

### Atributos


### run():

chequeamos tiempo inicial
int it = 1;

while (corriendo):
    iteracion_del_server(it);

    it = 0;
    control del framerate.
    it++

---

iteracion_del_server(it):

// broadcasts
{iniciamos broadcasts vacios}

// clientconnection handler
{vaciar cola de clientes a agregar}
{vaciar cola de clientes a eliminar}

// command handler
{vaciar cola de comandos recibidos}

// turnos de juego
{hacemos los turnos players.act monsters.act bla bla bla}(it)

// persist
{si es necesario, persistir}

// enviar broadcasts
{push broadcast a las colas de las clientconnections}

