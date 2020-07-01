#ifndef __CLIENT_H__
#define __CLIENT_H__

//-----------------------------------------------------------------------------
#include <stdio.h>  // debugging
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Sistemas de SDL
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include "SDL/Mixer.h"
#include "SDL/Renderer.h"
#include "SDL/Window.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../../Common/includes/BlockingQueue.h"
#include "../../Common/includes/DataStructs.h"
#include "../../Common/includes/Exceptions/Exception.h"
#include "../../Common/includes/JSON.h"
#include "../../Common/includes/NonBlockingQueue.h"
#include "../../Common/includes/Protocol.h"
#include "../../Common/includes/Socket/SocketWrapper.h"
#include "../../Common/includes/json_conversion.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "contexts.h"
#include "paths.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "ConnectionView.h"
#include "GameModel/Broadcasts/Broadcast.h"
#include "GameModel/CommandDispatcher.h"
#include "GameModel/Commands/Command.h"
#include "GameModel/Messages/Message.h"
#include "GameModel/Receiver.h"
#include "GameView/GameView.h"
#include "HomeView.h"
#include "SignUpView.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class Client {
   private:
    // Componentes SDL
    Window window;
    Renderer renderer;

    // Contexto actual
    Context current_context;

    // Conexión
    SocketWrapper socket;

    //-----------------------------------------------------------------------------
    // Métodos privados

    /* Inicializa sistemas de SDL */
    void _initSDL();

    /* Inicializa componentes internas */
    void _initComponents();

    //-----------------------------------------------------------------------------
    // Contextos

    void _launchHomeCtx();
    void _launchConnectionCtx();
    void _launchSignUpCtx();
    void _launchGameCtx();

    //-----------------------------------------------------------------------------
    // Auxiliares

    /* Recibe el primer paquete del GameCtx */
    void _receiveFirstPackage(NonBlockingQueue<Broadcast*>& broadcasts) const;

    /* Finaliza ordenadamente los recursos del GameCtx */
    void _finishGameCtx(BlockingQueue<Command*>& commands,
                        CommandDispatcher& command_dispatcher,
                        Receiver& receiver);

    //-----------------------------------------------------------------------------

    /* Cierra sistemas de SDL */
    void _quitSDL();

    //-----------------------------------------------------------------------------

   public:
    /* Constructor */
    Client();

    /* Deshabilitamos el constructor por copia. */
    Client(const Client&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    Client& operator=(const Client&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    Client(Client&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    Client& operator=(Client&& other) = delete;

    //-----------------------------------------------------------------------------
    // Métodos de la API pública

    /* Ejecuta el cliente */
    void launch();

    //-----------------------------------------------------------------------------

    /* Destructor */
    ~Client();
};

//-----------------------------------------------------------------------------

#endif  // __CLIENT_H__
