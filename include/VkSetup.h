//
// Created by wakiaki on 1/13/26.
//

#ifndef VULKANGAMES2DBASE_VKSETUP_H
#define VULKANGAMES2DBASE_VKSETUP_H

class VkSetup
{
public:
    VkSetup() = default;
    ~VkSetup() {};

    void Cleanup();

    bool InitVulkan();

    inline const std::unique_ptr<vk::raii::Instance>& GetInstance() const { return mVulkanInstance; }

private:

    bool CreateInstance();
    bool CheckExtensions(const vk::raii::Context& vContext, const uint32_t& extensionCount, const char** extensions);

    std::unique_ptr<vk::raii::Instance> mVulkanInstance;
    vk::raii::Context mContext;
    std::vector<const char*> mInstanceExtensions;
};


#endif //VULKANGAMES2DBASE_VKSETUP_H
