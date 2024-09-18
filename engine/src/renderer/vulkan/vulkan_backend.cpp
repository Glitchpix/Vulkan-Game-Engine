#include "vulkan_backend.hpp"
#include "core/logger.hpp"
#include "vulkan_device.hpp"
#include "vulkan_platform.hpp"

#include <cstddef>
#include <memory>
#include <vulkan/vulkan_core.h>


VulkanRenderer::VulkanRenderer(const char* applicationName, Platform* platform) : RendererBackend(platform, RendererBackend::BackendType::RENDERER_BACKEND_TYPE_VULKAN) {
#if defined(_DEBUG)
    mEnableValidationLayers = true;
#endif
    MSG_TRACE("Vulkan Renderer: %p created", this);
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = applicationName;
    appInfo.applicationVersion = VK_MAKE_API_VERSION(0, 0, 1, 0);
    appInfo.pEngineName = "Vulkan Test Engine";
    appInfo.engineVersion = VK_MAKE_API_VERSION(0, 0, 1, 0);
    appInfo.apiVersion = VK_API_VERSION_1_3;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    const auto requiredExtensions = get_required_extensions();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size());
    createInfo.ppEnabledExtensionNames = requiredExtensions.data();

    MSG_DEBUG("[Vulkan] %i validation layer", mEnableValidationLayers);
    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    if (mEnableValidationLayers) {
        mValidationLayers.emplace_back("VK_LAYER_KHRONOS_validation");

        // Verify validation layers can be used
        if (!check_validation_layer_support()) {
            MSG_ERROR("[Vulkan] Support for all validation layers failed, disabling validation support.");
            mEnableValidationLayers = false;
            mValidationLayers.clear();
        } else {
            MSG_INFO("[Vulkan] All required validation layers are supported");
        }
        populate_debug_messenger_create_info(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
    }
    size_t requiredValidationLayerCount = mValidationLayers.size();

    createInfo.enabledLayerCount = requiredValidationLayerCount;
    createInfo.ppEnabledLayerNames = mValidationLayers.data();  // TODO: Check this, might be unsafe
    createInfo.pNext = nullptr;

    //TODO: custom allocator
    VkResult result = vkCreateInstance(&createInfo, nullptr, &mInstance);

    if (result != VK_SUCCESS) {
        MSG_FATAL("Failed to create Vulkan renderer: %p", this);
        throw std::runtime_error("");
    }
    setup_debug_messenger();

    // Device setup
    mDevice = std::make_unique<VulkanDevice>(mInstance);

    MSG_TRACE("Vulkan Renderer: %p initialized", this);
};

VulkanRenderer::~VulkanRenderer() {
    MSG_DEBUG("Vulkan renderer: %p destructor called", this)
    if (mEnableValidationLayers) {
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(mInstance,
                                                                               "vkDestroyDebugUtilsMessengerEXT");
        if (func != nullptr) {
            func(mInstance, mDebugMessenger, nullptr);
        }
    }
    vkDestroyInstance(mInstance, nullptr);
}

void VulkanRenderer::resized(i16 width, i16 height) {
    //TODO
}

bool VulkanRenderer::begin_frame(f64 deltaTime) {
    //TODO
    return true;
}
bool VulkanRenderer::end_frame(f64 deltaTime) {
    //TODO
    return true;
}

std::vector<const char*> VulkanRenderer::get_required_extensions() {
    std::vector<const char*> extensions{};

    extensions.emplace_back(VK_KHR_SURFACE_EXTENSION_NAME);  // Generic surface

    std::vector<const char*> requiredPlatformExtensions = vulkanplatform::get_platform_extensions();

    for (const auto& extension : requiredPlatformExtensions) {
        extensions.emplace_back(extension);
    }

    if (mEnableValidationLayers) {
        extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        MSG_DEBUG("[Vulkan] Required extensions:");
        for (const auto& extension : extensions) {
            MSG_DEBUG(extension);
        }
    }

    return extensions;
}

bool VulkanRenderer::check_validation_layer_support() {
    uint32_t layerCount{};
    VK_CHECK(vkEnumerateInstanceLayerProperties(&layerCount, nullptr));

    std::vector<VkLayerProperties> availableLayers(layerCount);
    VK_CHECK(vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data()));

    MSG_TRACE("[Vulkan] Verifying validation layers...");
    for (const char* layerName : mValidationLayers) {
        MSG_TRACE("[Vulkan] Checking validation layer: %s", layerName);
        bool layerFound = false;
        for (const auto& layerProperties : availableLayers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                layerFound = true;
                MSG_TRACE("[Vulkan] Found validation layer: %s", layerName);
                break;
            }
        }

        if (!layerFound) {
            MSG_WARN("[Vulkan] Missing validation layer: %s", layerName);
            return false;
        }
    }

    return true;
}

void VulkanRenderer::populate_debug_messenger_create_info(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debug_callback;
}

void VulkanRenderer::setup_debug_messenger() {
    if (!mEnableValidationLayers) {
        return;
    }

    VkDebugUtilsMessengerCreateInfoEXT createInfo{};
    populate_debug_messenger_create_info(createInfo);

    VK_CHECK(CreateDebugUtilsMessengerEXT(mInstance, &createInfo, nullptr, &mDebugMessenger));
    MSG_DEBUG("[Vulkan] Debug messenger created");
}

VKAPI_ATTR VkBool32 VKAPI_CALL VulkanRenderer::debug_callback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData) {

    switch (messageSeverity) {
        default:
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            MSG_ERROR(pCallbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            MSG_WARN(pCallbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            MSG_INFO(pCallbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
            MSG_TRACE(pCallbackData->pMessage);
            break;
    }

    return VK_FALSE;
}

VkResult VulkanRenderer::CreateDebugUtilsMessengerEXT(VkInstance instance,
                                                      const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                                      const VkAllocationCallbacks* pAllocator,
                                                      VkDebugUtilsMessengerEXT* pDebugMessenger) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance,
                                                                          "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    }
    return VK_ERROR_EXTENSION_NOT_PRESENT;
}