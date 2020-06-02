# Abstracciones útiles

Se incluyen en este directorio varias de las abstracciones diseñadas en anteriores trabajos prácticos, a fines de utilizarlas sin tener que reescribir código de más. En cada una de ellas se deja un manual de uso para su utilización.

## Abstracciones integradas

* **Cliente/Servidor**
    * [Socket](./Client-Server/Socket/) - *método de comunicación entre procesos mediante protocolo TCP*
    * [Accepter](./Client-Server/Accepter/) - *hilo del server-side que acepta clientes*
* **Threads**
    * [Thread](./Threads/Thread/) - *clase abstracta de la que heredan los objetos activos.*
    * [Monitor](./Threads/Monitor/) - *recursos compartidos protegidos entre hilos.*
    * [ActiveObject](./Threads/ActiveObject/) - *objeto que corre en su propio hilo.*
    * [BlockingQueue](./Threads/BlockingQueue/) - *cola bloqueante que utiliza condition variables.*
* **Exceptions**
    * [Exception](./Exceptions/Exception/) - *hereda de std::exception, agrega mensajes de error formateados e información de errno*
        * [ParticularException](./Exceptions/ParticularException/) - *para devolver excepciones particulares*