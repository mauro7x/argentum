# Broadcast

{
    "new": {
        "characters": [],
        "creatures": [],
        "objects": []
    },
    "update": {
        "characters": [],
        "creatures": [],
        "objects": []
    },
    "delete": {
        "characters": [],
        "creatures": [],
        "objects": []
    }
}


Broadcast
map<id_map, json> packages

{
    "new": {
        "characters": [],
        "creatures": [],
        "objects": []
    },
    "update": {
        "characters": [],
        "creatures": [],
        "objects": []
    },
    "delete": {
        "characters": [],
        "creatures": [],
        "objects": []
    }
}


ServerLoop
- Queue de comandos que es única 
- Queue de broadcasteo.

- Una queue en cada sender de cada cliente.

---
sdfd

enviar respuestas por aca

dfgdfg
dfg

_sendWholeMapInfo() {

}

_sendBroadcast() {
    Notification* noti = NULL;
    while (noti = broadcastqueue.pop()) {

        active_clients.push()
    }

}

---

Broadcast : public Notification {

}


instance_id, map_id, data



CambiarDeMapaCmd::exec(game&, broadcast&, fullinfo&) {
    NotificationReply* reply = NULL;
    old_map_id = game.getMapId(id); 
    reply = game.changeMap(id, posicion)

    reply.sucess()
    si el comando se pudo hacer {

        // borra todo lo que tengas

        // agregar al jugador id a la cola de fullinfo para mandarle todo de ese mapa
        broadcast.push{id, old_map_id, enum=DELETE}
        broadcast.push{id, new_map_id, enum=CREATE}

    } si el comando no se pudo hacer {

    }
}

Client
switch new delete update
- current_map
mapview.select(map_id)
- contenedor de unidades
- contenedor de pjs
- contenedor de monstruos
- contenedor de objetos tirados en el suelo


enum: CHANGE_MAP
new_map_id: 34
{paquete con toda la info}


FullBroadcast(new_map_id, json);
full.setInfo(json);
full.send(peer) {
    peer << OPCODE;
    peer << new_map_id;
    peer << json
    ...
}

Queue<FullBroadcast*> queue;




Broadcast:


----> Cola de actualizaciones [updates]

    2 opciones:
        encolamos PLAYER IDS + TYPE [NEW, DELETE, UPDATE].

        encolamos EntityBroadcast



----> Cola de full broadcast [mandar toda la info]

    2 opciones:
        encolamos PLAYER IDS.

        encolamos EntityBroadcast


std::map<PlayerId, PlayerData>


enum type {NEW, UPDATE, DELETE}




    // Envio del broadcast
    _processUpdateBroadcasts() {
        // LOS DATOS SON DEFINITIVOS
        // desencolo NotificationBroadcst*
        // me fijo si ya lo habia procesado.
        // le pido el broadcast
        // lo pusheo a la cola de notificaciones de todos los senders
            // -> cada sender se fija si pertenece a su mapa y lo envia.
    }
    _processFullBroadcasts() {
        // LOS DATOS SON DEFINITIVOS
        // preparo el broadcast de todos los jugadores.
        // desencolo id
        // le pusheo al sender del ID que desencole.
    }


    ColaUpdates<PlayerBroadcastInfo*> info;



    while(!colaupdates.empty())
        info = colaupdates.pop()

        std::map<MapId, Broadcast>




    struct PlayerBroadcastInfo {
        BroadcastType = NEW/DELETE/UPDATE.
        PlayerData = ?.
    }

    struct Broadcast {
        
    }

J1 NEW toda la info playerdata
J2 NEW toda la info playerdata
J1 UPDATE x_coord +=1 playerdata
J2 UPDATE y_coord-=1 playerdata
J2 UPDATE vida -= 30 playerdata
J1 UPDATE ... playerdata
J1 UPDATE .. playerdata
J1 DELETE .. playerdata

{
        
}
    

    Desencolo y armo algo asi
     {
        map_id = 1: {
                        Jugador 1: UPDATE
                        Jugador 2: NEW
                        Jugador 3: 
                    }
        map_id = 2: {

                    }





    