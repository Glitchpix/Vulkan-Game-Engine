#pragma once
#include "defines.hpp"

#include <optional>
#include <vector>
#include <vulkan/vulkan.h>

class VulkanDevice {
public:
    VulkanDevice(VkInstance instance, VkSurfaceKHR surface);
    ~VulkanDevice();

private:
    VkInstance mInstance{nullptr};
    VkSurfaceKHR mSurface{nullptr};
    VkPhysicalDevice mPhysicalDevice{nullptr};
    VkPhysicalDeviceProperties mDeviceProperties;  // TODO: Fill in rest here
    VkDevice mDevice{nullptr};

    //TODO: Make this configurable
    const std::vector<const char*> mDeviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME};

    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;
        std::optional<uint32_t> computeFamily;
        std::optional<uint32_t> transferFamily;

        [[nodiscard]] bool is_complete() const {
            return graphicsFamily.has_value() &&
                presentFamily.has_value() &&
                computeFamily.has_value() &&
                transferFamily.has_value();
        }
    };

    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    void pick_physical_device();
    void create_logical_device();
    bool is_device_suitable(VkPhysicalDevice physicalDevice);
    bool check_device_extension_support(VkPhysicalDevice physicalDevice);
    QueueFamilyIndices find_queue_families(VkPhysicalDevice physicalDevice);
    SwapChainSupportDetails query_swapchain_support(VkPhysicalDevice physicalDevice);
};