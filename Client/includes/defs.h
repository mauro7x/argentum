#ifndef __CLIENT_DEFS_H__
#define __CLIENT_DEFS_H__

//-----------------------------------------------------------------------------
// Argumentos esperados en la invocación

enum ExpectedArgs { NAME, EXPECTED_ARGC };
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Códigos de retorno

#define SUCCESS 0
#define ERROR 1

#define USAGE_ERROR 100
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Configuración del frame-rate

#define FPS 60            /* frames/s */
#define RATE (1000 / FPS) /* ms/frame */
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Configuración de información de las unidades

#define INFO_SPACE_FROM_HEAD 10 /* px */
#define INFO_NAME_FONTSIZE 16   /* fontsize del nickname + lvl de c/ unidad */
#define INFO_LVL_FONTSIZE 10    /* fontsize del nickname + lvl de c/ unidad */
//-----------------------------------------------------------------------------

#endif  // __CLIENT_DEFS_H__
