#include "renderer.hpp"
#include "renderer_backend.hpp"
#include "vulkan/vulkan_backend.hpp"
#include "core/logger.hpp"

Renderer::Renderer(const char *applicationName, Platform *platform) {
    //TODO: make renderer configurable
    mRenderer = std::make_unique<VulkanRenderer>(VulkanRenderer(platform));
    if (!mRenderer->initialize(applicationName)){
        MSG_FATAL("Renderer failed creation!");
        //TODO: Throw here
    }
    MSG_TRACE("Renderer: %p created", this);
}

Renderer::~Renderer() = default;

void Renderer::on_resize(i16 width, i16 height) {
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
