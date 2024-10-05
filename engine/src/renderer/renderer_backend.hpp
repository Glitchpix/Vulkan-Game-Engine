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

    RendererBackend(const RendererBackend&) = default;
    RendererBackend(RendererBackend&&) = delete;
    RendererBackend& operator=(const RendererBackend&) = default;
    RendererBackend& operator=(RendererBackend&&) = delete;
    RendererBackend(Platform* platform, BackendType backendType, uint32_t width, uint32_t height)
        : mPlatform{platform}, mBackendType{backendType}, mWidth{width}, mHeight{height} {};
    virtual ~RendererBackend() = default;

    virtual void resized(uint32_t width, uint32_t height) = 0;

    virtual bool begin_frame(f64 deltaTime) = 0;
    virtual bool end_frame(f64 deltaTime) = 0;

protected:
    Platform* mPlatform;
    BackendType mBackendType;
    uint32_t mWidth{0};
    uint32_t mHeight{0};
};