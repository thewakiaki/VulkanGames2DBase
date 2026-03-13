//
// Created by wakiaki on 3/11/26.
//

#ifndef VULKANGAMES2DBASE_INDEXBUFFER_H
#define VULKANGAMES2DBASE_INDEXBUFFER_H
#include "VKRenderBuffer.h"

class VKCommandBuffer;

class IndexBuffer : public VKRenderBuffer {

public:

    bool SetupBuffers(const std::unique_ptr<VulkanLogicalDevice> &lDevice, const std::unique_ptr<VulkanPhysicalDevice> &pDevice, std::vector<uint16_t>& indices) override;
    bool CopyStagingData(const std::unique_ptr<VKCommandBuffer>& cmdBuffer, const std::unique_ptr<VulkanLogicalDevice>& lDevice) override;

    [[nodiscard]] const std::unique_ptr<vk::raii::Buffer>& GetBuffer() const override { return mIndexBuffer; }

    void Cleanup() override;

protected:
    bool SetupBuffer(const std::unique_ptr<VulkanLogicalDevice> &lDevice, CustomVKStructs::BufferType bufferType) override;
    bool SetupMemory(const std::unique_ptr<VulkanPhysicalDevice> &pDevice, const std::unique_ptr<VulkanLogicalDevice> &lDevice) override;

    vk::BufferCreateInfo SetBufferCreateInfo() override;
    [[nodiscard]] vk::MemoryAllocateInfo SetMemoryAllocateInfo() const override;

private:
    std::unique_ptr<vk::raii::Buffer> mIndexBuffer = nullptr;
    std::unique_ptr<vk::raii::DeviceMemory> mIndexMemory = nullptr;
    vk::MemoryRequirements mIndexMemoryRequirements;

    uint32_t mIndexMemoryTypeIndex = 0;
};


#endif //VULKANGAMES2DBASE_INDEXBUFFER_H