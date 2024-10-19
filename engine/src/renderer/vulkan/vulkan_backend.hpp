#pragma once

#include "defines.hpp"
#include "renderer/renderer_backend.hpp"

#include <memory>
#include <string>
#include <vector>
#include <vulkan/vulkan.h>

class VulkanDevice;
class VulkanSwapchain;
class RenderPass;
class VulkanCommandBuffer;

class VulkanRenderer : public RendererBackend {
public:
    VulkanRenderer(const VulkanRenderer&) = delete;
    VulkanRenderer(VulkanRenderer&&) = delete;
    VulkanRenderer& operator=(const VulkanRenderer&) = delete;
    VulkanRenderer& operator=(VulkanRenderer&&) = delete;
    VulkanRenderer(std::string applicationName, Platform* platform, uint32_t width, uint32_t height);
    ~VulkanRenderer() override;

    void resized(uint32_t width, uint32_t height) override;

    bool begin_frame(f64 deltaTime) override;
    bool end_frame(f64 deltaTime) override;

private:
    VkInstance mInstance{nullptr};
    VkSurfaceKHR mSurface{nullptr};
    VkDebugUtilsMessengerEXT mDebugMessenger{nullptr};
    std::unique_ptr<VulkanDevice> mDevice;
    std::unique_ptr<VulkanSwapchain> mSwapchain;
    std::unique_ptr<RenderPass> mRenderpass;
    std::vector<VulkanCommandBuffer> mCommandBuffers;

    bool mEnableValidationLayers{false};
    std::vector<const char*> mValidationLayers;

    std::vector<const char*> get_required_extensions();
    bool check_validation_layer_support();
    static void populate_debug_messenger_create_info(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    void setup_debug_messenger();
    void create_command_buffers();
    void destroy_command_buffers();

    static VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,
                                                 const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                                 const VkAllocationCallbacks* pAllocator,
                                                 VkDebugUtilsMessengerEXT* pDebugMessenger);
    static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                         VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                         const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                         void* pUserData);
};