#pragma once

#include "renderer/renderer_backend.hpp"
#include "defines.hpp"
#include <vector>
#include <vulkan/vulkan.h>
#include "core/logger.hpp"
#include "core/asserts.hpp"

#define VK_CHECK(expr)                    \
    {                                     \
        ENGINE_ASSERT(expr == VK_SUCCESS); \
    }                  
class VulkanRenderer : public RendererBackend {
public:
    VulkanRenderer(Platform* platform);
    virtual ~VulkanRenderer() = default;

    bool initialize(const char* applicationName) override;

    void shutdown() override;

    void resized(i16 width, i16 height) override;

    bool begin_frame(f64 deltaTime) override;
    bool end_frame(f64 deltaTime) override;
private:
    VkInstance mInstance{nullptr};
    bool mEnableValidationLayers{false};
    std::vector<const char*> mValidationLayers;
    
    std::vector<const char*> get_required_extensions();
    bool check_validation_layer_support();
};