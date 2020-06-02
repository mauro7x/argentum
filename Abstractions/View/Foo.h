#ifndef __FOO_H__
#define __FOO_H__

//-----------------------------------------------------------------------------
#include "Entity.h"
#include "Texture.h"
#include "config.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#define FOO_UP_FRAMES 4
#define FOO_DOWN_FRAMES 4
#define FOO_LEFT_FRAMES 4
#define FOO_RIGHT_FRAMES 4

#define FOO_ROWS 4
#define FOO_COLS 4
#define FOO_WIDTH 256
#define FOO_HEIGHT 256

// Animation speed en FPS
#define FOO_ANIMATION_SPEED 15
#define FOO_MOVEMENT_SPEED 100

enum FooAnimationState { FOO_NONE, FOO_DOWN, FOO_UP, FOO_LEFT, FOO_RIGHT };
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class Foo : public Entity {
   private:
    Texture mSpriteSheetTexture;

    SDL_Rect mWalkUpClips[FOO_UP_FRAMES];
    SDL_Rect mWalkDownClips[FOO_DOWN_FRAMES];
    SDL_Rect mWalkLeftClips[FOO_LEFT_FRAMES];
    SDL_Rect mWalkRightClips[FOO_RIGHT_FRAMES];

    FooAnimationState mCurrentAnimationState;
    SDL_Rect* mCurrentClip;
    int current_frame;

    Uint32 change_step;
    Uint32 last_changed;

    // Position
    int mPosX;
    int mPosY;
    int mVelX;
    int mVelY;

    // Dimensions
    int mWidth;
    int mHeight;

   public:
    /* Constructor */
    Foo(SDL_Renderer* renderer, int pPosX, int pPosY);

    //-------------------------------------------------------------------------
    // Interfaz genérica de Entidad

    /* Inicializar la media necesaria */
    void loadMedia() override;

    /* Manejar evento del usuario */
    void handleEvent(const SDL_Event& e) override;

    /* Acción en cada frame */
    void act() override;

    /* Renderizarse */
    void render() const override;

    /* Liberar sus recursos (SI ES NECESARIO) */
    void free() override;

    //-------------------------------------------------------------------------

    /* Destructor */
    ~Foo();
};

//-----------------------------------------------------------------------------

#endif  // __FOO_H__
