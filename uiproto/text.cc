#include "text.hh"

#include "font.h"

Text::Text(SDL_Renderer* ren, int font_size)
    : ren_(ren)
{
    TTF_Init();
    SDL_RWops* font_mem = SDL_RWFromMem(font, font_len);
    font_ = TTF_OpenFontRW(font_mem, 1, font_size);
    lineskip = TTF_FontLineSkip(font_);
}

Text::~Text()
{
    TTF_CloseFont(font_);
    TTF_Quit();
}

TextTexture Text::text_tx(std::string const& text, SDL_Color const& color) const
{
    clear_cache();

    if (const auto it = cache_.find(text); it != cache_.end())
        return { .tx = it->second.texture.get(), .w = it->second.w, .h = it->second.h, .lineskip = lineskip };

    SDL_Surface* sf = TTF_RenderText_Blended(font_, text.c_str(), color);
    SDL_Texture* tx = SDL_CreateTextureFromSurface(ren_, sf);
    SDL_FreeSurface(sf);
    TextTexture r { .tx = tx };
    SDL_QueryTexture(tx, nullptr, nullptr, &r.w, &r.h);
    cache_[text] = {
        .texture = std::unique_ptr<SDL_Texture, TextureDeleter>{ tx },
        .w = r.w,
        .h = r.h,
        .last_used = SDL_GetTicks64()
    };
    return { .tx = tx, .w = r.w, .h = r.h, .lineskip = lineskip };
}

void Text::clear_cache() const
{
    ++call_count_;
    if (call_count_ % 1000 == 0) {
        const uint64_t now = SDL_GetTicks64();
        for (auto it = cache_.begin(); it != cache_.end();) {
            if (it->second.last_used < (now - (CACHE_KEEP_SECONDS * 1000)))
                it = cache_.erase(it);
            else
                ++it;
        }
    }
}

int Text::write(SDL const& sdl, std::string const& text, int x, int y) const
{
    const auto [tx, tw, th, lineskip] = text_tx(text, { 0, 0, 0, SDL_ALPHA_OPAQUE });
    const SDL_Rect r { x, y, tw, th };
    SDL_RenderCopy(sdl.ren, tx, nullptr, &r);
    return y + lineskip;
}
