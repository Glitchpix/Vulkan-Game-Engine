#include "vulkan_device.hpp"
#include "core/logger.hpp"
#include "vulkan/vulkan_core.h"
#include "vulkan_defines.inl"
#include <set>
#include <vector>

VulkanDevice::VulkanDevice(VkInstance instance, VkSurfaceKHR surface) : mInstance{instance}, mSurface{surface} {
    //TODO: Implement
    pick_physical_device();
    MSG_INFO("[Vulkan] Device: %p initialized", this);
}

VulkanDevice::~VulkanDevice() {
    //TODO: Implement
    MSG_INFO("[Vulkan] Device: %p destroyed", this);
}

void VulkanDevice::pick_physical_device() {
    uint32_t deviceCount = 0;
    VK_CHECK(vkEnumeratePhysicalDevices(mInstance, &deviceCount, nullptr));

    if (deviceCount == 0) {
        throw std::runtime_error("failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    VK_CHECK(vkEnumeratePhysicalDevices(mInstance, &deviceCount, devices.data()));

    for (const auto& device : devices) {
        if (is_device_suitable(device)) {
            mPhysicalDevice = device;
            break;
        }
    }

    if (mPhysicalDevice == VK_NULL_HANDLE) {
        throw std::runtime_error("failed to find a suitable GPU!");
    }
}

bool VulkanDevice::is_device_suitable(VkPhysicalDevice physicalDevice) {
    //TODO: Create temporary DeviceProperties object and only assign to mDevice if successful
    vkGetPhysicalDeviceProperties(physicalDevice, &mDeviceProperties);
    MSG_INFO("[Vulkan] Device: %s queried, checking requirements...", mDeviceProperties.deviceName);

    // TODO: Save as member if successful
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceFeatures(physicalDevice, &deviceFeatures);

    // TODO: Save as member if successful
    VkPhysicalDeviceMemoryProperties memoryProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);

    QueueFamilyIndices indices = find_queue_families(physicalDevice);

    bool extensionsSupported = check_device_extension_support(physicalDevice);

    bool swapChainAdequate = false;
    if (extensionsSupported) {
        SwapChainSupportDetails swapChainSupport = query_swapchain_support(physicalDevice);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }

    if (mDeviceProperties.deviceType != VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && (deviceFeatures.geometryShader != VK_TRUE)) {
        MSG_INFO("[Vulkan] Device: %s is not a discrete GPU and does not support geometry shaders!", mDeviceProperties.deviceName);
        return false;
    }

    if (!indices.is_complete()) {
        MSG_INFO("[Vulkan] Device: %s does not support all required queue families!", mDeviceProperties.deviceName);
        return false;
    }

    if (!extensionsSupported) {
        MSG_INFO("[Vulkan] Device: %s does not support all required extensions!", mDeviceProperties.deviceName);
        return false;
    }

    if (!swapChainAdequate) {
        MSG_INFO("[Vulkan] Device: %s does not not have an adequate swapchain!", mDeviceProperties.deviceName);
        return false;
    }

    if (deviceFeatures.samplerAnisotropy != VK_TRUE) {
        MSG_INFO("[Vulkan] Device: %s does not not support anisotropy!", mDeviceProperties.deviceName);
        return false;
    }

    MSG_INFO("[Vulkan] Device: %s fulfills all requirements and is suitable", mDeviceProperties.deviceName);

    MSG_INFO("Selected device: '%s'.", mDeviceProperties.deviceName);
    // GPU type, etc.
    switch (mDeviceProperties.deviceType) {
        default:
        case VK_PHYSICAL_DEVICE_TYPE_OTHER:
            MSG_INFO("GPU type is Unknown.");
            break;
        case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
            MSG_INFO("GPU type is Integrated.");
            break;
        case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
            MSG_INFO("GPU type is Descrete.");
            break;
        case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
            MSG_INFO("GPU type is Virtual.");
            break;
        case VK_PHYSICAL_DEVICE_TYPE_CPU:
            MSG_INFO("GPU type is CPU.");
            break;
    }
    MSG_INFO(
        "GPU Driver version: %d.%d.%d",
        VK_VERSION_MAJOR(mDeviceProperties.driverVersion),
        VK_VERSION_MINOR(mDeviceProperties.driverVersion),
        VK_VERSION_PATCH(mDeviceProperties.driverVersion));
    // Vulkan API version.
    MSG_INFO(
        "Vulkan API version: %d.%d.%d",
        VK_VERSION_MAJOR(mDeviceProperties.apiVersion),
        VK_VERSION_MINOR(mDeviceProperties.apiVersion),
        VK_VERSION_PATCH(mDeviceProperties.apiVersion));
    // Memory information
    // TODO: Refactor some of these magic numbers
    for (auto j = 0; j < memoryProperties.memoryHeapCount; ++j) {
        f32 memory_size_gib = (((f32)memoryProperties.memoryHeaps[j].size) / 1024.0F / 1024.0F / 1024.0F);
        if ((memoryProperties.memoryHeaps[j].flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) != VK_FALSE) {
            MSG_INFO("Local GPU memory: %.2f GiB", memory_size_gib);
        } else {
            MSG_INFO("Shared System memory: %.2f GiB", memory_size_gib);
        }
    }
    return true;
}

bool VulkanDevice::check_device_extension_support(VkPhysicalDevice physicalDevice) {
    uint32_t extensionCount = 0;
    vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(mDeviceExtensions.begin(), mDeviceExtensions.end());

    for (const auto& extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}
/* 
void VulkanDevice::create_logical_device() {
    QueueFamilyIndices indices = findQueueFamilies(mPhysicalDevice);

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(),
                                              indices.presentFamily.value()};

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures{};
    deviceFeatures.samplerAnisotropy = VK_TRUE;

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();

    createInfo.pEnabledFeatures = &deviceFeatures;

    createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();

    if (enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    } else {
        createInfo.enabledLayerCount = 0;
    }

    if (vkCreateDevice(mPhysicalDevice, &createInfo, nullptr, &mDevice) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create logical device");
    }

    vkGetDeviceQueue(mPhysicalDevice, indices.graphicsFamily.value(), 0, &graphicsQueue);
    vkGetDeviceQueue(mDevice, indices.presentFamily.value(), 0, &presentQueue);
}*/

VulkanDevice::QueueFamilyIndices VulkanDevice::find_queue_families(VkPhysicalDevice physicalDevice) {
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

    int i{0};
    int minTransferScore{255};
    MSG_INFO("[Vulkan] Selected device queue families");
    MSG_INFO("Graphics | Present | Compute | Transfer | Name");
    for (const auto& queueFamily : queueFamilies) {
        int currentTransferScore{0};

        if ((queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) != VK_FALSE) {
            indices.graphicsFamily = i;
            ++currentTransferScore;
        }

        if ((queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT) != VK_FALSE) {
            indices.computeFamily = i;
            ++currentTransferScore;
        }

        if ((queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT) != VK_FALSE) {
            if (currentTransferScore < minTransferScore) {
                minTransferScore = currentTransferScore;
                indices.transferFamily = i;
            }
        }

        VkBool32 presentSupport = VK_FALSE;
        VK_CHECK(vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, mSurface, &presentSupport));
        if (presentSupport == VK_TRUE) {
            indices.presentFamily = i;
        }

        if (indices.is_complete()) {
            break;
        }
        i++;
    }

    MSG_INFO("%d | %d | %d | %d | %s",
             indices.graphicsFamily.value_or(-1),
             indices.presentFamily.value_or(-1),
             indices.computeFamily.value_or(-1),
             indices.transferFamily.value_or(-1),
             mDeviceProperties.deviceName);

    return indices;
}

VulkanDevice::SwapChainSupportDetails VulkanDevice::query_swapchain_support(VkPhysicalDevice physicalDevice) {
    SwapChainSupportDetails details{};

    VK_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, mSurface, &details.capabilities));

    // Format
    uint32_t formatCount = 0;
    VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, mSurface, &formatCount, nullptr));

    if (formatCount != 0) {
        details.formats.resize(formatCount);
        VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, mSurface, &formatCount, details.formats.data()));
    }

    // Present
    uint32_t presentModeCount = 0;
    VK_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, mSurface, &presentModeCount, nullptr));

    if (presentModeCount != 0) {
        details.presentModes.resize(presentModeCount);
        VK_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, mSurface, &presentModeCount, details.presentModes.data()));
    }

    return details;
}