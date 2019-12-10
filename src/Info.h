#ifndef Info_h
#define Info_h

#include <vulkan/vulkan.h>
#include <vector>

class Info {
    struct Layer{
        VkLayerProperties properties;
        std::vector<VkExtensionProperties> instance_extensions;
        std::vector<std::vector<VkExtensionProperties>> device_extensions;
    };
public:
    Info();
    ~Info();
    void print_all() const;
private:
    static VkResult initOS();
    static bool m_initiatedOS;

    VkResult get_layers();
    VkResult init_instance(char const *const app_name);
    VkResult check_gpus();
    VkResult describe_layers();
    VkResult describe_instance_extensions(Layer& l);
    VkResult describe_device_extensions(Layer& l);

    void print_instance() const;
    void print_gpus() const;
    void print_layers() const;
    void print_layer(const VkLayerProperties& layer) const;
    void print_extensions(const std::vector<VkExtensionProperties> &extensions) const;
    void print_extention(const VkExtensionProperties &ext) const;

    VkInstance m_inst;
    std::vector<VkPhysicalDevice> m_gpus;
    std::vector<Layer> m_layers;
};

#endif // Info_h
