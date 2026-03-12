//
// Created by wakiaki on 3/11/26.
//

#ifndef VULKANGAMES2DBASE_RENDERBUFFER_H
#define VULKANGAMES2DBASE_RENDERBUFFER_H
#include "CustomVkStructs.h"

class CustomLD;
class CustomPD;
class CmdBuffer;

class RenderBuffer {
public:
    virtual ~RenderBuffer() = default;

    virtual bool SetupBuffers(const std::unique_ptr<CustomLD>& lDevice, const std::unique_ptr<CustomPD>& pDevice, std::vector<CustomVKStructs::Vertex>& vertices);
    virtual bool SetupBuffers(const std::unique_ptr<CustomLD>& lDevice, const std::unique_ptr<CustomPD>& pDevice, std::vector<uint16_t>& indices);
    virtual bool CopyStagingData(const std::unique_ptr<CmdBuffer>& cmdBuffer, const std::unique_ptr<CustomLD>& lDevice) { return true; };

    [[nodiscard]] virtual const std::unique_ptr<vk::raii::Buffer>& GetBuffer() const { return mStagingBuffer; }


    virtual void Cleanup();

protected:
    virtual bool SetupBuffer(const std::unique_ptr<CustomLD>& lDevice, CustomVKStructs::BufferType bufferType = CustomVKStructs::BufferType::STAGING_BUFFER);
    virtual bool SetupMemory(const std::unique_ptr<CustomPD>& pDevice, const std::unique_ptr<CustomLD>& lDevice);

    virtual vk::BufferCreateInfo SetBufferCreateInfo();
    virtual vk::MemoryAllocateInfo SetMemoryAllocateInfo() const;

    uint32_t FindMemoryTypeIndex(const std::unique_ptr<CustomPD>& pDevice, const uint32_t& typeFilter, vk::MemoryPropertyFlags properties) const;

    std::unique_ptr<vk::raii::Buffer> mStagingBuffer = nullptr;
    std::unique_ptr<vk::raii::DeviceMemory> mStagingDeviceMemory = nullptr;

    vk::MemoryRequirements mStagingMemoryRequirements;
    vk::DeviceSize mVertexBufferSize = 0;
    vk::DeviceSize mIndexBufferSize = 0;
    vk::DeviceSize mStageCreateInfoSize = 0;

    uint32_t mStagingMemoryTypeIndex = 0;

    const uint32_t mNoSuitableDeviceMemoryType = std::numeric_limits<uint32_t>::max();

    void BindStagingMemory(const std::vector<CustomVKStructs::Vertex>& vertices) const;
    void BindStagingMemory(const std::vector<uint16_t>& indices) const;

private:
    vk::BufferCreateInfo SetStageVertexCreateInfo();
    vk::BufferCreateInfo SetStageIndexCreateInfo();

};


#endif //VULKANGAMES2DBASE_RENDERBUFFER_H