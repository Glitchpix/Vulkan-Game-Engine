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

    Renderer(Platform* platform, BackendType backendType) : mPlatform{platform}, mBackendType{backendType} {};

    virtual bool initialize(const char* applicationName, Platform* platform) = 0;

    virtual void shutdown() = 0;

    virtual void resized(i16 width, i16 height) = 0;

    virtual bool begin_frame(f64 deltaTime) = 0;
    virtual bool end_frame(f64 deltaTime) = 0;

protected:
    Platform* mPlatform;
    BackendType mBackendType;
};