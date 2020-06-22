#include "../../includes/View/ProgressBar.h"

//-----------------------------------------------------------------------------
// Métodos privados

void ProgressBar::_renderProgress() const {
    float relative_progress = ((float)current_value) / max_value;

    SDL_Rect render_quad = render_rect;
    render_quad.w *= relative_progress;
    g_renderer->fillQuad(&render_quad, color.r, color.g, color.b, color.a);
}
void ProgressBar::_center(SDL_Point& texture_pos, const Texture& texture,
                          const SDL_Rect& rect) {
    texture_pos.x = rect.x + (rect.w - texture.getWidth()) / 2;
    texture_pos.y = rect.y + (rect.h - texture.getHeight()) / 2;
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

ProgressBar::ProgressBar(const Renderer* g_renderer)
    : g_renderer(g_renderer), font(NULL) {}

void ProgressBar::init(const SDL_Point& render_offset, uint32_t current_value,
                       uint32_t max_value, const SDL_Color& color,
                       ProgressBarTextType text_type) {
    if (initialized) {
        throw Exception("ProgressBar already initialized.");
    }

    this->color = color;
    render_rect.x = render_offset.x;
    render_rect.y = render_offset.y;
    this->current_value = current_value;
    this->max_value = max_value;
    this->text_type = text_type;

    initialized = true;
}

void ProgressBar::loadMedia() {
    if (!initialized) {
        throw Exception("ProgressBar not initialized.");
    }

    // Cargamos la fuente
    font = TTF_OpenFont(FONT_OLDLONDON_FP, PROGRESS_BAR_FONT_SIZE);

    if (!font) {
        throw Exception("ProgressBar::loadMedia: Error opening TTF_Font/s.");
    }

    // Cargamos las texturas estáticas
    base.loadFromFile(g_renderer, HUD_PROGRESSBAR_BASE_FP);
    bisel.loadFromFile(g_renderer, HUD_PROGRESSBAR_BISEL_FP);
    border.loadFromFile(g_renderer, HUD_PROGRESSBAR_BORDER_FP);

    // Cargamos el texto
    if (text_type) {
        text.loadFromRenderedText(g_renderer, font, "loading",
                                  SDL_Color({0xFF, 0xFF, 0xFF, 0xFF}));
        _center(text_pos, text, render_rect);
    }
}

void ProgressBar::update(uint32_t current_value, uint32_t max_value) {
    if (!initialized) {
        throw Exception("ProgressBar not initialized.");
    }

    if ((this->current_value != current_value) ||
        (this->max_value != max_value)) {
        this->current_value = current_value;
        this->max_value = max_value;

        switch (text_type) {
            case ABSOLUTE_PBTEXTYPE: {
                text.loadFromRenderedText(g_renderer, font,
                                          std::to_string(current_value) + "/" +
                                              std::to_string(max_value),
                                          SDL_Color({0xFF, 0xFF, 0xFF, 0xFF}));
                break;
            }

            case PORCENTUAL_PBTEXTYPE: {
                text.loadFromRenderedText(
                    g_renderer, font,
                    std::to_string(
                        (int)((((float)current_value) / max_value) * 100)) +
                        "%",
                    SDL_Color({0xFF, 0xFF, 0xFF, 0xFF}));
                break;
            }

            case BOTH_PBTEXTYPE: {
                text.loadFromRenderedText(
                    g_renderer, font,
                    std::to_string(current_value) + "/" +
                        std::to_string(max_value) + " (" +
                        std::to_string(
                            (int)((((float)current_value) / max_value) * 100)) +
                        "%)",
                    SDL_Color({0xFF, 0xFF, 0xFF, 0xFF}));
                break;
            }

            case NONE_PBTEXTYPE: {
                break;
            }
        }

        if (text_type) {
            _center(text_pos, text, render_rect);
        }
    }
}

void ProgressBar::render() const {
    if (!initialized) {
        throw Exception("ProgressBar not initialized.");
    }

    // Renderizamos la progress bar por partes
    SDL_Rect render_quad = render_rect;
    g_renderer->render(base.getTexture(), &render_quad);
    _renderProgress();

    render_quad = render_rect;
    g_renderer->render(bisel.getTexture(), &render_quad);

    render_quad = render_rect;
    g_renderer->render(border.getTexture(), &render_quad);

    // Renderizamos el texto
    if (text_type) {
        render_quad = {text_pos.x, text_pos.y, text.getWidth(),
                       text.getHeight()};
        g_renderer->render(text.getTexture(), &render_quad);
    }
}

void ProgressBar::free() {
    if (font) {
        TTF_CloseFont(font);
        font = NULL;
    }
}

ProgressBar::~ProgressBar() {
    free();
}

//-----------------------------------------------------------------------------
