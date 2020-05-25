# Socket

Wrapper de socket para facilitar su uso. Ofrece tres constructores: uno para el socket accepter del servidor, uno para los peer sockets del servidor, y otro para el cliente. Ofrece sobrecarga de operadores >> y << para la recepción y el envío de distintos tipos de datos.

## Abstracciones necesarias

Se utilizan las siguientes abstracciones necesarias:

* Exception
    * ClosedSocketException