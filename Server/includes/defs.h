#ifndef __SERVER_DEFS_H__
#define __SERVER_DEFS_H__

//-----------------------------------------------------------------------------
// Argumentos esperados en la invocación

enum ExpectedArgs { NAME, PORT, EXPECTED_ARGC };
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Códigos de retorno

#define SUCCESS 0
#define ERROR 1

#define USAGE_ERROR 100
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Configuración del servidor

#define MAX_CLIENTS_QUEUED 10
#define FPS 30            /* frames/s */
#define RATE (1000 / FPS) /* ms/frame */
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Modelo

#define CRITICAL_ATTACK_DAMAGE_MODIFIER 2
#define TIME_TO_UPDATE_ATTRIBUTES 1000  // ms
//-----------------------------------------------------------------------------

#endif  // __SERVER_DEFS_H__
