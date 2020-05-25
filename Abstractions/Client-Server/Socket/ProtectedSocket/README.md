# ProtectedSocket

Socket identico al implementado en [RegularSocket](../RegularSocket/) pero protegido con un mutex. Soporta multi-threading.

## Abstracciones necesarias

Se utilizan las siguientes abstracciones necesarias:

* Exception
    * ClosedSocketException