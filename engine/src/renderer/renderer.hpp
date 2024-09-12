#pragma once

#include "defines.hpp"

class Platform;

class Renderer {
public:
    enum class BackendType {
        RENDERER_BACKEND_TYPE_VULKAN,
        RENDERER_BACKEND_TYPE_OPENGL,
        RENDERER_BACKEND_TYPE_DIRECTX
    };

    bool (*initialize)(const char* applicationName, Platform* platform);

    void (*shutdown)();

    void (*resized)(i16 width, i16 height);

    bool (*begin_frame)(f64 deltaTime);
    bool (*end_frame)(f64 deltaTime);

private:
    Platform* mPlatform;
};