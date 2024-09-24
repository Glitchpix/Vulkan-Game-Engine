#include "renderer.hpp"
#include "core/logger.hpp"
#include "renderer_backend.hpp"
#include "vulkan/vulkan_backend.hpp"


Renderer::Renderer(std::string applicationName, Platform *platform) {
    //TODO: make renderer configurable
    mRenderer = std::make_unique<VulkanRenderer>(applicationName, platform);
    MSG_TRACE("Renderer: %p created", static_cast<void *>(this));
}

Renderer::~Renderer() = default;

void Renderer::on_resize(i16, i16) {
    //TODO: handle resize
    MSG_ERROR("Resize not implemented yet!");
}

bool Renderer::draw_frame(RenderPacket &renderPacket) {
    if (!mRenderer->begin_frame(renderPacket.deltaTime)) {
        MSG_WARN("Frame failed to start rendering");
        return false;
    };
    if (!mRenderer->end_frame(renderPacket.deltaTime)) {
        MSG_WARN("Frame failed to finish rendering")
        return false;
    }
    return true;
}
