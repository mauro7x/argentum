# Abstracciones útiles

Se incluyen en este directorio varias de las abstracciones diseñadas en anteriores trabajos prácticos, a fines de utilizarlas sin tener que reescribir código de más. En cada una de ellas se deja un manual de uso para su utilización.

## Abstracciones integradas

* **Cliente/Servidor**
    * [Socket](./Client-Server/Socket/) - *método de comunicación entre procesos mediante protocolo TCP*
    * [Accepter]() - *hilo del server-side que acepta clientes*
* **Threads**
    * [Thread]() - *clase abstracta de la que heredan los objetos activos.*
    * [Monitor]() - *recursos compartidos protegidos entre hilos.*
    * [ActiveObject]() - *objeto que corre en su propio hilo.*
    * [BlockingQueue]() - *cola bloqueante que utiliza condition variables.*
* **Exceptions**
    * [Exception]() - *hereda de std::exception, agrega mensajes de error y códigos de error*
        * [ParticularException]() - *para devolver excepciones particulares*