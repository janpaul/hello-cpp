#pragma once

#include <tuple>
#include <SDL3/SDL.h>

namespace hello {
    [[maybe_unused]]
    constexpr SDL_FColor toFColor(const Uint8 r, const Uint8 g, const Uint8 b, const Uint8 a = 255) {
        return SDL_FColor{
            static_cast<float>(r) / 255.0f, static_cast<float>(g) / 255.0f, static_cast<float>(b) / 255.0f,
            static_cast<float>(a) / 255.0f
        };
    }

    [[maybe_unused]]
    constexpr SDL_FColor toFColor(const SDL_Color &c) {
        return toFColor(c.r, c.g, c.b, c.a);
    }

    using WindowSize = std::pair<float, float>;
    [[maybe_unused]]
    inline WindowSize getWindowSize(SDL_Window *win) {
        int w, h;
        SDL_GetWindowSizeInPixels(win, &w, &h);
        return { static_cast<float>(w), static_cast<float>(h) };
    }
}