#ifndef TEXT_HH
#define TEXT_HH

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

struct TextTexture {
    SDL_Texture* tx;
    int w, h;
    int lineskip;
};

class Text {

    struct TextureDeleter {
        void operator()(SDL_Texture* tx) const { SDL_DestroyTexture(tx); }
    };

    struct CachedText {
        std::unique_ptr<SDL_Texture, TextureDeleter> texture;
        int w, h;
        uint64_t last_used;
    };

public:
    explicit Text(SDL_Renderer* ren, int font_size);
    ~Text();

    [[nodiscard]] TextTexture text_tx(std::string const& text, SDL_Color const& color);

private:
    void clear_cache();

    TTF_Font* font_ = nullptr;
    SDL_Renderer* ren_ = nullptr;
    std::unordered_map<std::string, CachedText> cache_;
    size_t call_count_ = 0;
    int    lineskip = 0;

    static constexpr uint64_t CACHE_KEEP_SECONDS = 10;
};

#endif //TEXT_HH
