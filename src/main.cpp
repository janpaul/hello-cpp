#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <functional>
#include <memory>
#include <print>
#include <ranges>
#include <stdexcept>
#include <unordered_map>

#include <SDL3/SDL.h>

#include "helpers.hpp"

std::unordered_map<SDL_Keycode, std::function<void()> > keymap;

class SdlGuard {
public:
    explicit SdlGuard(const Uint32 flags) {
        if (!SDL_Init(flags)) {
            throw std::runtime_error(SDL_GetError());
        }
    }

    ~SdlGuard() noexcept { SDL_Quit(); }

    SdlGuard(const SdlGuard &) = delete;

    SdlGuard &operator=(const SdlGuard &) = delete;

    SdlGuard(SdlGuard &&) = delete;

    SdlGuard &operator=(SdlGuard &&) = delete;
};

using WindowPtr = std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)>;
using RendererPtr = std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)>;

auto dispatchKey(const SDL_Keycode key) {
    if (const auto it = keymap.find(key); it != keymap.end())
        it->second();
}

int main() try {
    SdlGuard sdl{SDL_INIT_VIDEO};

    WindowPtr window{
        SDL_CreateWindow("SDL3 window", 800, 450, SDL_WINDOW_RESIZABLE),
        &SDL_DestroyWindow
    };
    if (!window) {
        SDL_Log("SDL_CreateWindow failed: %s", SDL_GetError());
        return 1;
    }

    RendererPtr renderer{
        SDL_CreateRenderer(window.get(), nullptr),
        &SDL_DestroyRenderer
    };
    if (!renderer) {
        SDL_Log("SDL_CreateRenderer failed: %s", SDL_GetError());
        return 1;
    }

    const auto startTicks{SDL_GetTicks()};
    auto running{true};
    std::array<SDL_Vertex, 3> vertices{};
    keymap = {
        {SDLK_ESCAPE, [&running] { running = false; }},
        {SDLK_Q, [&running] { running = false; }},
    };

    const auto aColor{hello::toFColor(200, 100, 50)};

    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_EVENT_QUIT: {
                    running = false;
                    break;
                }
                case SDL_EVENT_KEY_DOWN: [[likely]] {
                    dispatchKey(e.key.key);
                    break;
                }
                default:
                    break;
            }
        }
        const auto ren{renderer.get()};
        const auto t{(static_cast<float>(SDL_GetTicks() - startTicks)) / 1000.0f};
        const auto angle{t};
        const auto [w, h]{hello::getWindowSize(window.get())};

        constexpr std::array base{
            SDL_FPoint{0.f, -100.f},
            SDL_FPoint{-100.f, 100.f},
            SDL_FPoint{100.f, 100.f}
        };
        SDL_SetRenderDrawColor(ren, 30, 30, 30, 255);
        SDL_RenderClear(ren);

        const float cx{static_cast<float>(w) / 2.0f}, cy{static_cast<float>(h) / 2.0f};
        const auto _sin = std::sin(angle);
        const auto _cos = std::cos(angle);
        std::ranges::for_each(std::views::iota(static_cast<size_t>(0), base.size()), [&](const auto i) {
            assert(i < base.size());
            const auto [x,y] = base[i];
            const float fx{x * _cos - y * _sin};
            const float fy{x * _sin + y * _cos};
            vertices[i].position.x = cx + fx;
            vertices[i].position.y = cy + fy;
            vertices[i].color = aColor;
            vertices[i].tex_coord = {0.f, 0.f};
        });

        SDL_RenderGeometry(ren, nullptr, vertices.data(), vertices.size(), nullptr, 0);
        SDL_RenderPresent(ren);
        SDL_Delay(16);
    }
    return 0;
} catch (const std::exception &ex) {
    SDL_Log("Fatal: %s", ex.what());
    return 1;
}
