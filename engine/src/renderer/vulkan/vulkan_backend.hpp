#pragma once

#include "renderer/renderer.hpp"
#include "defines.hpp"
#include <vulkan/vulkan.h>

class VulkanRenderer : public Renderer {
public:
    VulkanRenderer(Platform* platform) : Renderer(platform, Renderer::BackendType::RENDERER_BACKEND_TYPE_VULKAN) {};

    bool initialize(const char* applicationName, Platform* platform) override;

    void shutdown() override;

    void resized(i16 width, i16 height) override;

    bool begin_frame(f64 deltaTime) override;
    bool end_frame(f64 deltaTime) override;
private:
    VkInstance mInstance;
};