#include "vulkan_backend.hpp"
#include "core/logger.hpp"
#include "vulkan/vulkan_core.h"
#include <cstddef>

VulkanRenderer::VulkanRenderer(Platform *platform) : RendererBackend(platform, RendererBackend::BackendType::RENDERER_BACKEND_TYPE_VULKAN) {
    MSG_TRACE("Vulkan Renderer: %p created", this);
};

bool VulkanRenderer::initialize(const char *applicationName)
{
#if defined(_DEBUG)
    mEnableValidationLayers = true;
#endif
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
    if(mEnableValidationLayers) {
        mValidationLayers.emplace_back("VK_LAYER_KHRONOS_validation");

        // Verify validation layers can be used
        if (!check_validation_layer_support()){
            MSG_ERROR("[Vulkan] Support for all validation layers failed, disabling validation support.");
            mEnableValidationLayers = false;
            mValidationLayers.clear();
        } else {
            MSG_INFO("[Vulkan] All required validation layers are supported");
        }
    }
    size_t requiredValidationLayerCount = mValidationLayers.size();

    createInfo.enabledLayerCount = requiredValidationLayerCount;
    createInfo.ppEnabledLayerNames = mValidationLayers.data(); // TODO: Check this, might be unsafe
    createInfo.pNext = nullptr;

    //TODO: custom allocator
    VkResult result = vkCreateInstance(&createInfo, nullptr, &mInstance);

    if(result != VK_SUCCESS){
        MSG_ERROR("Failed to create Vulkan instance!");
        return false;
    }
    MSG_TRACE("Vulkan Renderer: %p initialized", this);
    return true;
}

void VulkanRenderer::shutdown() {
    //TODO
}

void VulkanRenderer::resized(i16 width, i16 height){
    //TODO
}

bool VulkanRenderer::begin_frame(f64 deltaTime){
    //TODO
    return true;
}
bool VulkanRenderer::end_frame(f64 deltaTime) {
    //TODO
    return true;
}

std::vector<const char*> VulkanRenderer::get_required_extensions(){  
    std::vector<const char*> extensions{};

    extensions.emplace_back(VK_KHR_SURFACE_EXTENSION_NAME); // Generic surface

    std::vector<const char*> requiredPlatformExtensions = mPlatform->get_required_extensions();

    for (const auto& extension : requiredPlatformExtensions) {
        extensions.emplace_back(extension);
    }

    if (mEnableValidationLayers){
        extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        MSG_DEBUG("[Vulkan] Required extensions:");
        for (const auto& extension : extensions) {
            MSG_DEBUG(extension);
        }
    }

    return extensions;
}

bool VulkanRenderer::check_validation_layer_support(){
    uint32_t layerCount{};
    VK_CHECK(vkEnumerateInstanceLayerProperties(&layerCount, nullptr));

    std::vector<VkLayerProperties> availableLayers(layerCount);
    VK_CHECK(vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data()));

    MSG_TRACE("[Vulkan] Verifying validation layers...");
    for (const char* layerName : mValidationLayers){
        MSG_TRACE("[Vulkan] Checking validation layer: %s", layerName);
        bool layerFound = false;
        for (const auto& layerProperties : availableLayers){
            if (strcmp(layerName, layerProperties.layerName) == 0){
                layerFound = true;
                MSG_TRACE("[Vulkan] Found validation layer: %s", layerName);
                break;
            }
        }

        if (!layerFound){
            MSG_WARN("[Vulkan] Missing validation layer: %s", layerName);
            return false;
        }
    }

    return true;
}