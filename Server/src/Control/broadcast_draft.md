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
