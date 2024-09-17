#pragma once

#include "defines.hpp"

class Platform;

class RendererBackend {
public:
    enum class BackendType {
        RENDERER_BACKEND_TYPE_VULKAN,
        RENDERER_BACKEND_TYPE_OPENGL,
        RENDERER_BACKEND_TYPE_DIRECTX
    };

    RendererBackend(Platform* platform, BackendType backendType) : mPlatform{platform}, mBackendType{backendType} {};
    virtual ~RendererBackend() = default;

    virtual void resized(i16 width, i16 height) = 0;

    virtual bool begin_frame(f64 deltaTime) = 0;
    virtual bool end_frame(f64 deltaTime) = 0;

protected:
    Platform* mPlatform;
    BackendType mBackendType;
};