#pragma once

#include "renderer/renderer_backend.hpp"
#include "defines.hpp"
#include <vulkan/vulkan.h>
#include "core/logger.hpp"

class VulkanRenderer : public RendererBackend {
public:
    VulkanRenderer(Platform* platform);
    ~VulkanRenderer() = default;

    bool initialize(const char* applicationName) override;

    void shutdown() override;

    void resized(i16 width, i16 height) override;

    bool begin_frame(f64 deltaTime) override;
    bool end_frame(f64 deltaTime) override;
private:
    VkInstance mInstance;
};