// Copyright 2020 Bastian Kuolt
#if 0
class Font;
class Text;

using SharedFont = std::shared_ptr<Font>;
using SharedText = std::shared_ptr<Text>;

class Font : protected std::enable_shared_from_this<Font> {
 public:
    friend class Text;

    Font(const std::filesystem::path &path, std::size_t size);
    virtual ~Font() noexcept;

    SharedText createText(const std::string &text = {});

 private:
    const std::size_t _size;
    TTF_Font *_handle { nullptr };
};

class Text {
 public:
    friend class Font;
    explicit Text(const SharedFont &font, const std::string &text = {});
    virtual ~Text() noexcept;

    void setText(const std::string &text);
    void render();

 private:
   const SharedFont _font;

   std::string _text;
   glm::ivec2 _dimensions;
   SDL_Texture *_texture { nullptr };
};


namespace {

std::once_flag is_TTF_initialized;

void initialize_TTF() {
    if (TTF_Init() == -1) {
        throw std::runtime_error { "could not initialize SDL TTF" };
    }
    std::atexit(TTF_Quit);
}

SDL_Renderer* getRenderer() {
    SDL_Renderer *renderer { SDL_GetRenderer(App.window->getHandle()) };
    if (renderer == nullptr) {
        throw std::runtime_error { "could not get SDL Renderer" };
    }
    return renderer;
}

}  // namespace

Font::Font(const std::filesystem::path &path, std::size_t size)
    : _size { size } {
    std::call_once(is_TTF_initialized, &initialize_TTF);
    _handle = TTF_OpenFont(path.string().c_str(), _size);
    if (_handle == nullptr) {
        throw std::runtime_error { "could not load font" };
    }
}

Font::~Font() noexcept {
    TTF_CloseFont(_handle);
}

SharedText Font::createText(const std::string &text) {
    return std::make_shared<Text>(shared_from_this(), text);
}

/* --------------------------------- Text --------------------------------- */

Text::Text(const SharedFont &font, const std::string &text)
    : _font { font } {
//    setText(text);
}

Text::~Text() noexcept {
    if (_texture != nullptr) {
        SDL_DestroyTexture(_texture);
    }
}

SDL_Texture* getSDLTexture(SDL_Surface *surface) {
    SDL_Texture *texture { SDL_CreateTextureFromSurface(getRenderer(), surface) };
    if (texture == nullptr) {
        throw std::runtime_error { SDL_GetError() };
    }
    return texture;
}

void Text::setText(const std::string &text) {
    const SDL_Color color { 255, 255, 255 };

    if (_texture) {  // delete previous texture
        SDL_DestroyTexture(_texture);
        _texture = nullptr;
    }

    SDL_Surface *surface = TTF_RenderText_Solid(_font->_handle, text.c_str(), color);
    if (surface == nullptr) {
        throw std::runtime_error { TTF_GetError() };
    }

    _texture = getSDLTexture(surface);
    SDL_FreeSurface(surface);
    if (_texture == nullptr) {
        throw std::runtime_error { TTF_GetError() };
    }

    // recalculate bounding box
    _text = text;
    if (TTF_SizeText(_font->_handle, _text.c_str(), &_dimensions.x, &_dimensions.y) == -1) {
        throw std::runtime_error { TTF_GetError() };
    }
}

void Text::render() {
    SDL_Renderer *renderer { getRenderer() };
    const SDL_Rect rectangle { 0, 0, _dimensions.x, _dimensions.y };
    if (SDL_RenderCopy(renderer, _texture, nullptr, &rectangle) == -1) {
        throw std::runtime_error { "could not render font" };
    }
}
#endif  // 0