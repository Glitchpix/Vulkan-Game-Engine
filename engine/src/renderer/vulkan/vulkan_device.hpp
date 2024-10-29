#pragma once
#include "defines.hpp"

#include <optional>
#include <vector>
#include <vulkan/vulkan.h>

class VulkanDevice {
    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;
        std::optional<uint32_t> computeFamily;
        std::optional<uint32_t> transferFamily;

        [[nodiscard]] bool is_complete() const {
            return graphicsFamily.has_value() && presentFamily.has_value() && computeFamily.has_value() &&
                transferFamily.has_value();
        }
    };

public:
    VulkanDevice(const VulkanDevice&) = default;
    VulkanDevice(VulkanDevice&&) = delete;
    VulkanDevice& operator=(const VulkanDevice&) = delete;
    VulkanDevice& operator=(VulkanDevice&&) = delete;
    VulkanDevice(VkInstance instance, VkSurfaceKHR surface, const std::vector<const char*>& validationLayers);
    ~VulkanDevice();
    [[nodiscard]] const SwapChainSupportDetails& get_swapchain_support_details() const {
        return mSwapChainSupport;
    }
    [[nodiscard]] VkDevice get_logical_device() const {
        return mDevice;
    }
    [[nodiscard]] const VkSurfaceKHR& get_surface() const {
        return mSurface;
    }
    [[nodiscard]] const QueueFamilyIndices& get_queue_families() const {
        return mQueueFamiles;
    }
    [[nodiscard]] const VkFormat& get_depth_format() const {
        return mDepthFormat;
    }
    [[nodiscard]] const VkPhysicalDeviceMemoryProperties& get_memory_properties() const {
        return mDeviceMemoryProperties;
    }
    [[nodiscard]] const VkCommandPool& get_graphics_commandpool() const {
        return mGraphicsCommandPool;
    }

    [[nodiscard]] const VkQueue& get_graphics_queue() const {
        return mGraphicsQueue;
    }

    [[nodiscard]] const VkQueue& get_present_queue() const {
        return mPresentQueue;
    }

private:
    VkInstance mInstance{nullptr};
    VkSurfaceKHR mSurface{nullptr};
    VkPhysicalDevice mPhysicalDevice{nullptr};
    VkPhysicalDeviceProperties mDeviceProperties{};
    VkPhysicalDeviceFeatures mDeviceFeatures{};
    VkPhysicalDeviceMemoryProperties mDeviceMemoryProperties{};
    SwapChainSupportDetails mSwapChainSupport{};
    QueueFamilyIndices mQueueFamiles{};
    VkDevice mDevice{nullptr};
    VkQueue mGraphicsQueue{nullptr};
    VkQueue mPresentQueue{nullptr};
    VkQueue mTransferQueue{nullptr};
    VkCommandPool mGraphicsCommandPool{nullptr};
    VkFormat mDepthFormat{};

    std::vector<const char*> mValidationLayers;

    //TODO: Make this configurable
    const std::vector<const char*> mDeviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};


    void pick_physical_device();
    void create_logical_device();

    bool is_device_suitable(VkPhysicalDevice physicalDevice);
    bool check_device_extension_support(VkPhysicalDevice physicalDevice);

    bool query_device_depth_format(VkPhysicalDevice physicalDevice, VkFormat& format);
    QueueFamilyIndices find_queue_families(VkPhysicalDevice physicalDevice);
    SwapChainSupportDetails query_swapchain_support(VkPhysicalDevice physicalDevice);
};