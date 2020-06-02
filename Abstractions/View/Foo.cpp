#include "Foo.h"

//-----------------------------------------------------------------------------
// Métodos privados
#include <iostream>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

Foo::Foo(SDL_Renderer* renderer, int pPosX, int pPosY)
    : Entity(renderer),
      mSpriteSheetTexture(renderer),
      mCurrentAnimationState(FOO_NONE),
      mCurrentClip(&mWalkDownClips[0]),
      current_frame(0),
      change_step(1000 / FOO_ANIMATION_SPEED),
      last_changed(0),
      mPosX(pPosX),
      mPosY(pPosY),
      mVelX(0),
      mVelY(0),
      mWidth(FOO_WIDTH / FOO_COLS),
      mHeight(FOO_HEIGHT / FOO_ROWS) {}

void Foo::loadMedia() {
    mSpriteSheetTexture.loadFromFile("../sprites.png");

    for (int i = 0; i < FOO_DOWN_FRAMES; i++) {
        mWalkDownClips[i].x = i * mWidth;
        mWalkDownClips[i].y = mHeight * 0;
        mWalkDownClips[i].w = mWidth;
        mWalkDownClips[i].h = mHeight;
    }

    for (int i = 0; i < FOO_LEFT_FRAMES; i++) {
        mWalkLeftClips[i].x = i * mWidth;
        mWalkLeftClips[i].y = mHeight * 1;
        mWalkLeftClips[i].w = mWidth;
        mWalkLeftClips[i].h = mHeight;
    }

    for (int i = 0; i < FOO_RIGHT_FRAMES; i++) {
        mWalkRightClips[i].x = i * mWidth;
        mWalkRightClips[i].y = mHeight * 2;
        mWalkRightClips[i].w = mWidth;
        mWalkRightClips[i].h = mHeight;
    }

    for (int i = 0; i < FOO_UP_FRAMES; i++) {
        mWalkUpClips[i].x = i * mWidth;
        mWalkUpClips[i].y = mHeight * 3;
        mWalkUpClips[i].w = mWidth;
        mWalkUpClips[i].h = mHeight;
    }
}

void Foo::handleEvent(const SDL_Event& e) {
    if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
        switch (e.key.keysym.sym) {
            case SDLK_w:
                mCurrentAnimationState = FOO_UP;
                mVelY -= FOO_MOVEMENT_SPEED;
                break;
            case SDLK_s:
                mCurrentAnimationState = FOO_DOWN;
                mVelY += FOO_MOVEMENT_SPEED;
                break;
            case SDLK_a:
                mCurrentAnimationState = FOO_LEFT;
                mVelX -= FOO_MOVEMENT_SPEED;
                break;
            case SDLK_d:
                mCurrentAnimationState = FOO_RIGHT;
                mVelX += FOO_MOVEMENT_SPEED;
                break;
        }
        current_frame = 0;
    } else if (e.type == SDL_KEYUP && e.key.repeat == 0) {
        switch (e.key.keysym.sym) {
            case SDLK_w:
                if (mCurrentAnimationState == FOO_UP) {
                    mCurrentAnimationState = FOO_NONE;
                }
                mVelY += FOO_MOVEMENT_SPEED;
                break;
            case SDLK_s:
                if (mCurrentAnimationState == FOO_DOWN) {
                    mCurrentAnimationState = FOO_NONE;
                }
                mVelY -= FOO_MOVEMENT_SPEED;
                break;
            case SDLK_a:
                if (mCurrentAnimationState == FOO_LEFT) {
                    mCurrentAnimationState = FOO_NONE;
                }
                mVelX += FOO_MOVEMENT_SPEED;
                break;
            case SDLK_d:
                if (mCurrentAnimationState == FOO_RIGHT) {
                    mCurrentAnimationState = FOO_NONE;
                }
                mVelX -= FOO_MOVEMENT_SPEED;
                break;
        }
    }
}

void Foo::act() {
    Uint32 ticks = SDL_GetTicks();
    float time_step = ticks - last_changed;
    std::cout << "PosX = " << mPosX << " | PosY = " << mPosY << "\n";
    if (time_step > change_step) {
        mPosX += mVelX * (time_step / 1000);

        if (mPosX < (mCurrentClip->w) / 2) {
            mPosX = (mCurrentClip->w) / 2;
        } else if (mPosX > WINDOW_WIDTH - (mCurrentClip->w) / 2) {
            mPosX = WINDOW_WIDTH - (mCurrentClip->w) / 2;
        }

        mPosY += mVelY * (time_step / 1000);
        if (mPosY < (mCurrentClip->h) / 2) {
            mPosY = (mCurrentClip->h) / 2;
        } else if (mPosY > WINDOW_HEIGHT - (mCurrentClip->h) / 2) {
            mPosY = WINDOW_HEIGHT - (mCurrentClip->h) / 2;
        }

        switch (mCurrentAnimationState) {
            case FOO_NONE: {
                break;
            }

            case FOO_UP: {
                mCurrentClip = &mWalkUpClips[current_frame];
                if (++current_frame >= FOO_UP_FRAMES) {
                    current_frame = 0;
                }
                break;
            }

            case FOO_DOWN: {
                mCurrentClip = &mWalkDownClips[current_frame];
                if (++current_frame >= FOO_DOWN_FRAMES) {
                    current_frame = 0;
                }
                break;
            }

            case FOO_LEFT: {
                mCurrentClip = &mWalkLeftClips[current_frame];
                if (++current_frame >= FOO_LEFT_FRAMES) {
                    current_frame = 0;
                }
                break;
            }

            case FOO_RIGHT: {
                mCurrentClip = &mWalkRightClips[current_frame];
                if (++current_frame >= FOO_RIGHT_FRAMES) {
                    current_frame = 0;
                }
                break;
            }
        }

        last_changed = ticks;
    }
}

void Foo::render() const {
    mSpriteSheetTexture.render(mPosX - (mCurrentClip->w) / 2,
                               mPosY - (mCurrentClip->h) / 2, mCurrentClip);
}

void Foo::free() {}

Foo::~Foo() {}

//-----------------------------------------------------------------------------
