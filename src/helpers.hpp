#pragma once

#include <SDL3/SDL.h>

struct WindowSize {
    int w;
    int h;
};

namespace hello {
    constexpr float kInv255 = 1.0f / 255.0f;

    [[nodiscard]] constexpr SDL_FColor toFColor(const Uint8 r, const Uint8 g, const Uint8 b, const Uint8 a = 255) noexcept {
        return SDL_FColor{
            static_cast<float>(r) * kInv255, static_cast<float>(g) * kInv255, static_cast<float>(b) * kInv255,
            static_cast<float>(a) * kInv255
        };
    }

    [[nodiscard]] constexpr SDL_FColor toFColor(const SDL_Color &c) noexcept {
        return toFColor(c.r, c.g, c.b, c.a);
    }

    [[nodiscard]] inline WindowSize getWindowSize(SDL_Window *win) noexcept{
        int w, h;
        SDL_GetWindowSizeInPixels(win, &w, &h);
        return { w,h };
    }
}