#pragma once
#include "defines.hpp"

#include <optional>
#include <vector>
#include <vulkan/vulkan.h>

class VulkanDevice {
public:
    VulkanDevice(const VulkanDevice &) = default;
    VulkanDevice(VulkanDevice &&) = delete;
    VulkanDevice &operator=(const VulkanDevice &) = delete;
    VulkanDevice &operator=(VulkanDevice &&) = delete;
    VulkanDevice(VkInstance instance, VkSurfaceKHR surface, const std::vector<const char *> &validationLayers);
    ~VulkanDevice();

private:
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

    VkInstance mInstance{nullptr};
    VkSurfaceKHR mSurface{nullptr};
    VkPhysicalDevice mPhysicalDevice{nullptr};
    VkPhysicalDeviceProperties mDeviceProperties{};
    VkPhysicalDeviceFeatures mDeviceFeatures{};
    VkPhysicalDeviceMemoryProperties mDeviceMemoryProperties{};
    SwapChainSupportDetails mSwapChainSupport{};
    VkDevice mDevice{nullptr};
    VkQueue mGraphicsQueue{nullptr};
    VkQueue mPresentQueue{nullptr};
    VkQueue mTransferQueue{nullptr};

    std::vector<const char *> mValidationLayers;

    //TODO: Make this configurable
    const std::vector<const char *> mDeviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME};


    void pick_physical_device();
    void create_logical_device();
    bool is_device_suitable(VkPhysicalDevice physicalDevice);
    bool check_device_extension_support(VkPhysicalDevice physicalDevice);
    QueueFamilyIndices find_queue_families(VkPhysicalDevice physicalDevice);
    SwapChainSupportDetails query_swapchain_support(VkPhysicalDevice physicalDevice);
};