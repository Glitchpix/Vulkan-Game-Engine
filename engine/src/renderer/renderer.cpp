#include "renderer.hpp"
#include "core/logger.hpp"
#include "renderer_backend.hpp"
#include "vulkan/vulkan_backend.hpp"


Renderer::Renderer(std::string applicationName, Platform* platform, i16 width, i16 height) {
    //TODO: make renderer configurable
    mRenderer = std::make_unique<VulkanRenderer>(applicationName, platform, width, height);
    MSG_TRACE("Renderer: {:p} created", static_cast<void*>(this));
}

Renderer::~Renderer() = default;

void Renderer::on_resize(i16 width, i16 height) {
    //TODO: handle resize
    MSG_ERROR("Resize not implemented yet!");
    mRenderer->resized(width, height);
}

bool Renderer::draw_frame(const RenderPacket& renderPacket) {
    if (!mRenderer->begin_frame(renderPacket.deltaTime)) {
        MSG_WARN("Frame failed to start rendering");
        return false;
    };
    if (!mRenderer->end_frame(renderPacket.deltaTime)) {
        MSG_WARN("Frame failed to finish rendering");
        return false;
    }
    return true;
}
