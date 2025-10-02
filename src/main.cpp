#include <memory>
#include <print>
#include <stdexcept>
#include <cmath>
#include <unordered_map>
#include <functional>
#include <SDL3/SDL.h>

#include "helpers.hpp"

std::unordered_map<SDL_Keycode, std::function<void()>> keymap;

class SdlGuard {
public:
    explicit SdlGuard(Uint32 flags) {
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
    std::array<SDL_Vertex, 3> verts{};
    keymap = {
        { SDLK_ESCAPE, [&running] { running = false; } },
        { SDLK_Q,      [&running] { running = false; } },
    };

    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_EVENT_QUIT: {
                    running = false;
                    break;
                }
                case SDL_EVENT_KEY_DOWN: {
                    auto it = keymap.find(e.key.key);
                    if (it != keymap.end()) {
                        it->second();
                    }
                    break;
                }
                default:
                    break;
            }
        }
        const auto ren{renderer.get()};

        const auto t{(static_cast<float>(SDL_GetTicks() - startTicks)) / 1000.0f};
        const auto angle{t};
        const auto [windowWidth, windowHeight] = hello::getWindowSize(window.get());

        SDL_FPoint base[3] = {
            {0.f, -100.f},
            {-100.f, 100.f},
            {100.f, 100.f}
        };
        SDL_SetRenderDrawColor(ren, 30, 30, 30, 255);
        SDL_RenderClear(ren);

        float cx{windowWidth / 2.0f}, cy{windowHeight / 2.0f};
        for (int i = 0; i < 3; i++) {
            float x{base[i].x * cos(angle) - base[i].y * sin(angle)};
            float y{base[i].x * sin(angle) + base[i].y * cos(angle)};
            verts[i].position.x = cx + x;
            verts[i].position.y = cy + y;
            verts[i].color = hello::toFColor(200, 100, 50);
            verts[i].tex_coord = {0.f, 0.f};
        }

        SDL_RenderGeometry(ren, nullptr, verts.data(), verts.size(), nullptr, 0);
        SDL_RenderPresent(ren);
        SDL_Delay(16);
    }
    return 0;
} catch (const std::exception &ex) {
    SDL_Log("Fatal: %s", ex.what());
    return 1;
}
