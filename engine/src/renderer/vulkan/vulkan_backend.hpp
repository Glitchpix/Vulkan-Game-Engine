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
    VulkanRenderer(const char* applicationName, Platform* platform);
    ~VulkanRenderer() override;

    void resized(i16 width, i16 height) override;

    bool begin_frame(f64 deltaTime) override;
    bool end_frame(f64 deltaTime) override;
private:
    VkInstance mInstance{nullptr};
    VkDebugUtilsMessengerEXT mDebugMessenger{nullptr};
    bool mEnableValidationLayers{false};
    std::vector<const char*> mValidationLayers;
    
    std::vector<const char*> get_required_extensions();
    bool check_validation_layer_support();
    static void populate_debug_messenger_create_info(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    void setup_debug_messenger();

    static VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,
                                      const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                      const VkAllocationCallbacks* pAllocator,
                                      VkDebugUtilsMessengerEXT* pDebugMessenger);
    static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData);
};