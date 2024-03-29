#include "Info.h"

#include <vulkan/vulkan.h>
#include <map>
#include <iostream>
#include <sstream>

#define stringify( name ) # name
#define VK_MAP_ITEM(item) {item, stringify( item )}

static const std::map<int, const char*> vk_result
{
    VK_MAP_ITEM(VK_SUCCESS),
            VK_MAP_ITEM(VK_NOT_READY),
            VK_MAP_ITEM(VK_TIMEOUT),
            VK_MAP_ITEM(VK_EVENT_SET),
            VK_MAP_ITEM(VK_EVENT_RESET),
            VK_MAP_ITEM(VK_INCOMPLETE),
            VK_MAP_ITEM(VK_ERROR_OUT_OF_HOST_MEMORY),
            VK_MAP_ITEM(VK_ERROR_OUT_OF_DEVICE_MEMORY),
            VK_MAP_ITEM(VK_ERROR_INITIALIZATION_FAILED),
            VK_MAP_ITEM(VK_ERROR_DEVICE_LOST),
            VK_MAP_ITEM(VK_ERROR_MEMORY_MAP_FAILED),
            VK_MAP_ITEM(VK_ERROR_LAYER_NOT_PRESENT),
            VK_MAP_ITEM(VK_ERROR_EXTENSION_NOT_PRESENT),
            VK_MAP_ITEM(VK_ERROR_FEATURE_NOT_PRESENT),
            VK_MAP_ITEM(VK_ERROR_INCOMPATIBLE_DRIVER),
            VK_MAP_ITEM(VK_ERROR_TOO_MANY_OBJECTS),
            VK_MAP_ITEM(VK_ERROR_FORMAT_NOT_SUPPORTED),
            VK_MAP_ITEM(VK_ERROR_FRAGMENTED_POOL),
            VK_MAP_ITEM(VK_ERROR_OUT_OF_POOL_MEMORY),
            VK_MAP_ITEM(VK_ERROR_INVALID_EXTERNAL_HANDLE),
            VK_MAP_ITEM(VK_ERROR_SURFACE_LOST_KHR),
            VK_MAP_ITEM(VK_ERROR_NATIVE_WINDOW_IN_USE_KHR),
            VK_MAP_ITEM(VK_SUBOPTIMAL_KHR),
            VK_MAP_ITEM(VK_ERROR_OUT_OF_DATE_KHR),
            VK_MAP_ITEM(VK_ERROR_INCOMPATIBLE_DISPLAY_KHR),
            VK_MAP_ITEM(VK_ERROR_VALIDATION_FAILED_EXT),
            VK_MAP_ITEM(VK_ERROR_INVALID_SHADER_NV),
            VK_MAP_ITEM(VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT),
            VK_MAP_ITEM(VK_ERROR_FRAGMENTATION_EXT),
            VK_MAP_ITEM(VK_ERROR_NOT_PERMITTED_EXT),
            VK_MAP_ITEM(VK_ERROR_INVALID_DEVICE_ADDRESS_EXT)
};

static const std::map<int, const char*> vk_queue_flags
{
    VK_MAP_ITEM( VK_QUEUE_GRAPHICS_BIT ),
            VK_MAP_ITEM( VK_QUEUE_COMPUTE_BIT ),
            VK_MAP_ITEM( VK_QUEUE_TRANSFER_BIT ),
            VK_MAP_ITEM( VK_QUEUE_SPARSE_BINDING_BIT ),
            VK_MAP_ITEM( VK_QUEUE_PROTECTED_BIT )
};

static const std::map<int, const char*> vk_device_types
{
    VK_MAP_ITEM( VK_PHYSICAL_DEVICE_TYPE_OTHER ),
            VK_MAP_ITEM( VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU ),
            VK_MAP_ITEM( VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU ),
            VK_MAP_ITEM( VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU ),
            VK_MAP_ITEM( VK_PHYSICAL_DEVICE_TYPE_CPU )
};

static const std::map<int, const char*> vk_memory_props_flags
{
    VK_MAP_ITEM( VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT ),
            VK_MAP_ITEM( VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT ),
            VK_MAP_ITEM( VK_MEMORY_PROPERTY_HOST_COHERENT_BIT ),
            VK_MAP_ITEM( VK_MEMORY_PROPERTY_HOST_CACHED_BIT ),
            VK_MAP_ITEM( VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT ),
            VK_MAP_ITEM( VK_MEMORY_PROPERTY_PROTECTED_BIT ),
            VK_MAP_ITEM( VK_MEMORY_PROPERTY_DEVICE_COHERENT_BIT_AMD ),
            VK_MAP_ITEM( VK_MEMORY_PROPERTY_DEVICE_UNCACHED_BIT_AMD )
};

static const std::map<int, const char*> vk_memory_heap_flags
{
    VK_MAP_ITEM( VK_MEMORY_HEAP_DEVICE_LOCAL_BIT ),
            VK_MAP_ITEM( VK_MEMORY_HEAP_MULTI_INSTANCE_BIT )
};

std::string substring(const std::string& str, size_t start, size_t from_end=0) {
    return str.substr(start, str.size()-start-from_end);
}

template<class I, class F, class M>
void extract_str(I index, F flags, const M& map, std::string& str, size_t start, size_t from_end=0) {
    if (flags & index) str.append(substring(map.at(index),start,from_end)).append("+");
}

std::string get_queue_flags(VkQueueFlags flags) {
    std::string f{"VK_QUEUE("};
    const size_t start{f.size()};
    const size_t from_end{4};
    extract_str(VK_QUEUE_GRAPHICS_BIT, flags, vk_queue_flags, f, start, from_end);
    extract_str(VK_QUEUE_COMPUTE_BIT, flags, vk_queue_flags, f, start, from_end);
    extract_str(VK_QUEUE_TRANSFER_BIT, flags, vk_queue_flags, f, start, from_end);
    extract_str(VK_QUEUE_SPARSE_BINDING_BIT, flags, vk_queue_flags, f, start, from_end);
    extract_str(VK_QUEUE_PROTECTED_BIT, flags, vk_queue_flags, f, start, from_end);
    if (f.size())  f.resize (f.size () - 1);
    return f.append(")");
}

std::string get_device_types(VkPhysicalDeviceType flags) {
    std::string f{"VK_PHYSICAL_DEVICE_TYPE("};
    const size_t start{f.size()};
    extract_str(VK_PHYSICAL_DEVICE_TYPE_OTHER, flags, vk_device_types, f, start);
    extract_str(VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU, flags, vk_device_types, f, start);
    extract_str(VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU, flags, vk_device_types, f, start);
    extract_str(VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU, flags, vk_device_types, f, start);
    extract_str(VK_PHYSICAL_DEVICE_TYPE_CPU, flags, vk_device_types, f, start);
    if (f.size())  f.resize (f.size () - 1);
    return f.append(")");
}

std::string get_memory_props_flags(VkMemoryPropertyFlags flags) {
    std::string f{"VK_MEMORY_PROPERTY("};
    const size_t start{f.size()};
    size_t from_end{4};
    extract_str(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, flags, vk_memory_props_flags, f, start, from_end);
    extract_str(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, flags, vk_memory_props_flags, f, start, from_end);
    extract_str(VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, flags, vk_memory_props_flags, f, start, from_end);
    extract_str(VK_MEMORY_PROPERTY_HOST_CACHED_BIT, flags, vk_memory_props_flags, f, start, from_end);
    extract_str(VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT, flags, vk_memory_props_flags, f, start, from_end);
    extract_str(VK_MEMORY_PROPERTY_PROTECTED_BIT, flags, vk_memory_props_flags, f, start, from_end);
    from_end+=4;
    extract_str(VK_MEMORY_PROPERTY_DEVICE_COHERENT_BIT_AMD, flags, vk_memory_props_flags, f, start, from_end);
    extract_str(VK_MEMORY_PROPERTY_DEVICE_UNCACHED_BIT_AMD, flags, vk_memory_props_flags, f, start, from_end);
    if (f.size())  f.resize (f.size () - 1);
    return f.append(")");
}

std::string get_memory_heap_flags(VkMemoryHeapFlags flags) {
    std::string f{"VK_MEMORY_HEAP("};
    const size_t start{f.size()};
    const size_t from_end{4};
    extract_str(VK_MEMORY_HEAP_DEVICE_LOCAL_BIT, flags, vk_memory_heap_flags, f, start, from_end);
    extract_str(VK_MEMORY_HEAP_MULTI_INSTANCE_BIT, flags, vk_memory_heap_flags, f, start, from_end);
    if (f.size())  f.resize (f.size () - 1);
    return f.append(")");
}

std::string uint8_array_to_str(const uint8_t* arr, size_t count) {
    std::stringstream sstream;
    sstream << std::hex << std::uppercase;
    for (size_t i = 0; i < count; ++i) {
        int num = arr[i];
        if (num < 16) sstream << "0";
        sstream << int(num);
        if (!((i+1)%4) && i != count-1) {
            sstream << "-";
        }
    }
    return sstream.str();
}

template<class T>
std::string bool32_array_to_str(T arr) {
    std::string f;
    size_t count{sizeof (arr) / sizeof(VkBool32)};
    VkBool32* fea = (VkBool32*)(&arr);
    for (size_t i = 0; i < count; ++i) {
        if (i && !(i%5)) {
            f.append(" ");
        }
        f.append(fea[i] ? "1" : "0");
    }
    return f;
}

bool Info::m_initiatedOS{false};

VkResult Info::initOS()
{
    if (!m_initiatedOS) {
#ifdef __ANDROID__
        // This place is the first place for samples to use Vulkan APIs.
        // Here, we are going to open Vulkan.so on the device and retrieve function pointers using
        // vulkan_wrapper helper.
        if (!InitVulkan()) {
            LOGE("Failied initializing Vulkan APIs!");
            return VK_ERROR_INITIALIZATION_FAILED;
        }
        LOGI("Loaded Vulkan APIs.");
#endif
        m_initiatedOS = true;
    }
    return VK_SUCCESS;
}

Info::Info() {
    VkResult result = initOS();
    if (VK_SUCCESS != result) {
        std::cout << "initOS error: " << vk_result.at(result) << "\n";
        return;
    }
    result = init_instance("VK_validator");
    if (VK_SUCCESS != result) {
        std::cout << "vkCreateInstance error: " << vk_result.at(result) << "\n";
        return;
    }
    result = check_gpus();
    if (VK_SUCCESS != result) {
        std::cout << "vkEnumeratePhysicalDevices error: " << vk_result.at(result) << "\n";
        return;
    }
    result = get_layers();
    if (VK_SUCCESS != result) {
        std::cout << "vkEnumerateInstanceLayerProperties error: " << vk_result.at(result) << "\n";
        return;
    }
    describe_layers();
}

Info::~Info() {
    vkDestroyInstance(m_inst, nullptr);
}

VkResult Info::init_instance(const char * const app_name) {
    VkApplicationInfo app_info = {};
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pNext = nullptr;
    app_info.pApplicationName = app_name;
    app_info.applicationVersion = 1;
    app_info.pEngineName = app_name;
    app_info.engineVersion = 1;
    app_info.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo inst_info = {};
    inst_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    inst_info.pNext = nullptr;
    inst_info.flags = 0;
    inst_info.pApplicationInfo = &app_info;
    inst_info.enabledLayerCount = 0;
    inst_info.ppEnabledLayerNames = nullptr;
    inst_info.enabledExtensionCount = 0;
    inst_info.ppEnabledExtensionNames = nullptr;

    return vkCreateInstance(&inst_info, nullptr, &m_inst);
}

VkResult Info::check_gpus() {
    uint32_t gpu_count{0};
    VkResult result = vkEnumeratePhysicalDevices(m_inst, &gpu_count, nullptr);
    if (VK_SUCCESS == result && gpu_count) {
        m_gpus.clear();
        std::vector<VkPhysicalDevice> gpus(gpu_count);
        result = vkEnumeratePhysicalDevices(m_inst, &gpu_count, gpus.data());
        if (VK_SUCCESS == result) {
            for(auto gpu : gpus) {
                m_gpus.push_back({gpu, {}});
            }
        }
    }
    if (VK_SUCCESS == result)
    {
        for (auto& gpu : m_gpus) {
            vkGetPhysicalDeviceProperties(gpu.device, &gpu.properties);
            uint32_t count{};
            vkGetPhysicalDeviceQueueFamilyProperties(gpu.device, &count, nullptr);
            if (count) {
                gpu.queue_props.resize(count);
                vkGetPhysicalDeviceQueueFamilyProperties(gpu.device, &count, gpu.queue_props.data());
            }
            vkGetPhysicalDeviceFeatures(gpu.device, &gpu.features);
            vkGetPhysicalDeviceMemoryProperties(gpu.device, &gpu.memory_props);
        }
    }
    return result;
}

VkResult Info::get_layers() {
    // only extensions provided by the Vulkan implementation or by implicitly enabled layers
    m_layers.push_back({});

    VkResult result;
    std::vector<VkLayerProperties> vk_props;

    do {
        uint32_t count;
        result = vkEnumerateInstanceLayerProperties(&count, nullptr);
        if (result) return result;

        if (count == 0) {
            return result;
        }

        vk_props.resize(count);

        result = vkEnumerateInstanceLayerProperties(&count, vk_props.data());
    } while (result == VK_INCOMPLETE);

    for (auto& l : vk_props) {
        m_layers.push_back({ {l}, {}, {}});
    }
    return result;
}

VkResult Info::describe_instance_extensions(Info::Layer &l) {
    VkResult result;

    do {
        uint32_t count{};
        // count extensions
        result = vkEnumerateInstanceExtensionProperties(l.properties.layerName, &count, nullptr);

        if (VK_SUCCESS == result) {
            if (count) {
                l.instance_extensions.resize(count);
                // get extensions descriptions
                result = vkEnumerateInstanceExtensionProperties(l.properties.layerName, &count, l.instance_extensions.data());
            }
        }
    }
    while (VK_INCOMPLETE == result);

    return result;
}

VkResult Info::describe_device_extensions(Info::Layer &l) {
    VkResult result{};
    l.device_extensions.resize(m_gpus.size());

    for (size_t i = 0; i < m_gpus.size(); ++i) {
        auto& ext = l.device_extensions.at(i);
        auto& gpu = m_gpus.at(i);
        do {
            uint32_t extensionCount{};
            // count extensions
            result = vkEnumerateDeviceExtensionProperties(gpu.device, l.properties.layerName, &extensionCount, nullptr);

            if (VK_SUCCESS == result) {
                if (extensionCount) {
                    ext.resize(extensionCount);
                    // get extensions descriptions
                    result = vkEnumerateDeviceExtensionProperties(gpu.device, l.properties.layerName, &extensionCount, ext.data());
                }
            }
        }
        while (VK_INCOMPLETE == result);

        if (VK_SUCCESS != result) break;
    }
    return result;
}

VkResult Info::describe_layers() {
    VkResult result{};
    for (auto& l : m_layers) {
        result = describe_instance_extensions(l);
        if (VK_SUCCESS != result) {
            break;
        }
        result = describe_device_extensions(l);
        if (VK_SUCCESS != result) {
            break;
        }
    }
    return result;
}

void Info::print_all() const {
    print_instance();
    print_gpus();
    print_layers();
}

void Info::print_instance() const
{
    std::cout << "====== Instance " << m_inst << "\n";
}

void Info::print_gpus() const
{
    std::cout << "====== " << m_gpus.size() << " GPU" << (m_gpus.size() == 1 ? "" : "s") << "\n";
    for (auto& gpu : m_gpus) {
        std::cout << "=== " << gpu.device << " | " << gpu.properties.deviceName
                  << " | api v." <<  gpu.properties.apiVersion
                  << " | driver v." <<  gpu.properties.driverVersion << "\n";
        std::cout << "=== https://pcilookup.com : " << std::hex << std::uppercase
                  << "vendorID " << gpu.properties.vendorID
                  << " | deviceID " << gpu.properties.deviceID << std::dec << std::nouppercase << "\n";
        std::cout << "=== types: " << get_device_types(gpu.properties.deviceType) << "\n";
        std::cout << "=== UUID: " << uint8_array_to_str(gpu.properties.pipelineCacheUUID, VK_UUID_SIZE) << "\n";
        std::cout << "=== limits: maxImageDimension1D=" << gpu.properties.limits.maxImageDimension1D
                  << ", maxImageDimension2D=" << gpu.properties.limits.maxImageDimension2D
                  << ", maxImageDimension3D=" << gpu.properties.limits.maxImageDimension3D
                  << ", maxImageDimensionCube=" << gpu.properties.limits.maxImageDimensionCube << ",...\n";
        std::cout << "=== sparse: " << bool32_array_to_str(gpu.properties.sparseProperties) << "\n";
        std::cout << "=== features: " << bool32_array_to_str(gpu.features) << "\n";

        std::cout << "=== memory:\n";
        for (uint32_t i = 0; i < gpu.memory_props.memoryTypeCount; ++i) {
            std::cout << gpu.memory_props.memoryTypes[i].heapIndex << ". "
                      << get_memory_props_flags(gpu.memory_props.memoryTypes[i].propertyFlags) << "\n";
        }
        for (uint32_t i = 0; i < gpu.memory_props.memoryHeapCount; ++i) {
            std::cout << "size=" << gpu.memory_props.memoryHeaps[i].size << " "
                      << get_memory_heap_flags(gpu.memory_props.memoryHeaps[i].flags) << "\n";
        }

        for (auto& p : gpu.queue_props) {
            std::cout << get_queue_flags(p.queueFlags) << " | count=" << p.queueCount << " | "
                      << "timestamp range=" << p.timestampValidBits << " | "
                      << "transfer granularity(w=" << p.minImageTransferGranularity.width
                      << ", h=" << p.minImageTransferGranularity.height
                      << ", d=" << p.minImageTransferGranularity.depth << ")\n";
        }
    }
}

void Info::print_layers() const
{
    for (auto& l : m_layers) {
        print_layer(l.properties);
        std::cout << "/// Instance Extensions: ";
        print_extensions(l.instance_extensions);
        std::cout << "/// Device Extensions ///\n";
        size_t i{};
        for (auto& d : l.device_extensions) {
            std::cout << "+ GPU " << m_gpus.at(i).device << " has ";
            print_extensions(d);
            ++i;
        }
    }

}

void Info::print_layer(const VkLayerProperties &layer) const
{
    std::cout << "===== Layer ";
    if (layer.layerName[0]) {
        std::cout << layer.layerName << "| spec v." << layer.specVersion
                  << " | imp v." << layer.implementationVersion << " /*" << layer.description << "*/ ";
    }
    else {
        std::cout << "TOP";
    }
    std::cout << " =====\n";
}

void Info::print_extensions(const std::vector<VkExtensionProperties>& extensions) const
{
    std::cout << extensions.size() << " extension" << (extensions.size() == 1 ? "" : "s") << "\n";
    for (auto& e : extensions) {
        print_extention(e);
    }
}

void Info::print_extention(const VkExtensionProperties& ext) const
{
    std::cout << ext.extensionName << ":\tver." << ext.specVersion << std::endl;
}
