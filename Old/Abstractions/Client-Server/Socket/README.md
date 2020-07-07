# Socket

Se ofrecen dos clases: Socket y SocketWrapper. Deberíamos usar siempre SocketWrapper ya que la idea de su implementación es ofrecer facilidades para el uso, como la sobrecarga de operadores >> y << para enviar distintos tipos de datos.

## Abstracciones necesarias

Se utilizan las siguientes abstracciones necesarias:

* Exception
    * ClosedSocketException