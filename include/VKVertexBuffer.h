//
// Created by wakiaki on 3/11/26.
//

#ifndef VULKANGAMES2DBASE_VERTEXBUFFER_H
#define VULKANGAMES2DBASE_VERTEXBUFFER_H
#include "VKRenderBuffer.h"

class VulkanLogicalDevice;
class VulkanPhysicalDevice;
class VKCommandBuffer;

class VKVertexBuffer : public VKRenderBuffer {

public:
    bool SetupBuffers(const std::unique_ptr<VulkanLogicalDevice>& lDevice, const std::unique_ptr<VulkanPhysicalDevice>& pDevice, std::vector<CustomVKStructs::Vertex>& vertices) override;
    bool CopyStagingData(const std::unique_ptr<VKCommandBuffer> &cmdBuffer, const std::unique_ptr<VulkanLogicalDevice>& lDevice) override;

    [[nodiscard]] const std::unique_ptr<vk::raii::Buffer>& GetBuffer() const override { return mVertexBuffer; }

    [[nodiscard]] const size_t& GetVertexCount() const { return mVertexCount; }

    void Cleanup() override;

protected:
    bool SetupBuffer(const std::unique_ptr<VulkanLogicalDevice> &lDevice, CustomVKStructs::BufferType bufferType) override;
    bool SetupMemory(const std::unique_ptr<VulkanPhysicalDevice> &pDevice, const std::unique_ptr<VulkanLogicalDevice> &lDevice) override;

    vk::BufferCreateInfo SetBufferCreateInfo() override;
    [[nodiscard]] vk::MemoryAllocateInfo SetMemoryAllocateInfo() const override;


private:
    std::unique_ptr<vk::raii::Buffer> mVertexBuffer = nullptr;
    std::unique_ptr<vk::raii::DeviceMemory> mVertexMemory = nullptr;
    vk::MemoryRequirements mVertexMemoryRequirements;

    uint32_t mVertexMemoryTypeIndex = 0;

    size_t mVertexCount = 0;
};


#endif //VULKANGAMES2DBASE_VERTEXBUFFER_H