#pragma once

#include "defines.hpp"
#include <memory>

class RendererBackend;
class Platform;

class Renderer {
public:
    struct RenderPacket {
        f64 deltaTime;
    };
    Renderer(const char* applicationName, Platform* platform);
    ~Renderer();

    void on_resize(i16 width, i16 height);
    bool draw_frame(RenderPacket& renderPacket);
private:
    std::unique_ptr<RendererBackend> mRenderer;
};