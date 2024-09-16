#include "vulkan_backend.hpp"
#include "core/logger.hpp"

VulkanRenderer::VulkanRenderer(Platform *platform) : RendererBackend(platform, RendererBackend::BackendType::RENDERER_BACKEND_TYPE_VULKAN) {
    MSG_TRACE("Vulkan Renderer: %p created", this);
};

bool VulkanRenderer::initialize(const char *applicationName)
{
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

    int numExtensions = 0; // TODO: get_required_extensions(); 
    createInfo.enabledExtensionCount = numExtensions;
    createInfo.ppEnabledExtensionNames = nullptr;
    createInfo.enabledLayerCount = 0;
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