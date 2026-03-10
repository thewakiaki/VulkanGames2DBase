//
// Created by wakiaki on 3/10/26.
//

#ifndef VULKANGAMES2DBASE_CUSTOMVERTEXBUFFER_H
#define VULKANGAMES2DBASE_CUSTOMVERTEXBUFFER_H

#include "CustomLD.h"

enum class BufferType
{
    VERTEX_BUFFER = 0,
    STAGING_BUFFER = 1
};

class CustomVertexBuffer {

public:

    bool SetupBuffers(const std::unique_ptr<CustomPD> &pDevice, const std::unique_ptr<CustomLD>& lDevice, const std::vector<CustomVKStructs::Vertex>& vertices);

    [[nodiscard]] const std::unique_ptr<vk::raii::Buffer>& GetVertexBuffer() const { return mBuffer; }

    void Cleanup();

private:
    bool SetupVertexBuffer(const std::unique_ptr<CustomPD> &pDevice, const std::unique_ptr<CustomLD>& lDevice, const std::vector<CustomVKStructs::Vertex>& vertices);
    bool SetupStagingVertexBuffer(const std::unique_ptr<CustomPD> &pDevice, const std::unique_ptr<CustomLD>& lDevice, const std::vector<CustomVKStructs::Vertex>& vertices);

    vk::BufferCreateInfo CreateBufferInfo(const std::unique_ptr<CustomLD>& lDevice, const std::vector<CustomVKStructs::Vertex>& vertices, BufferType bufferType);
    bool SetupBuffer(const std::unique_ptr<CustomLD>& lDevice, const std::vector<CustomVKStructs::Vertex>& vertices);
    bool SetupStagingBuffer(const std::unique_ptr<CustomLD>& lDevice, const std::vector<CustomVKStructs::Vertex>& vertices);

    bool SetupMemory(const std::unique_ptr<CustomPD>& pDevice, const std::unique_ptr<CustomLD>& lDevice);
    bool SetupStagingMemory(const std::unique_ptr<CustomPD>& pDevice, const std::unique_ptr<CustomLD>& lDevice);
    uint32_t FindDeviceMemoryType(const std::unique_ptr<CustomPD> &pDevice, uint32_t typeFilter, vk::MemoryPropertyFlags properties);
    vk::MemoryAllocateInfo CreateDeviceMemInfo(const uint32_t& memTypeIndex, BufferType bufferType);

    void FillBuffer(const std::vector<CustomVKStructs::Vertex>& vertices);

    std::unique_ptr<vk::raii::Buffer> mBuffer = nullptr;
    std::unique_ptr<vk::raii::DeviceMemory> mDeviceMemory = nullptr;
    vk::MemoryRequirements mMemoryRequirements;

    std::unique_ptr<vk::raii::Buffer> mStagingBuffer = nullptr;
    std::unique_ptr<vk::raii::DeviceMemory> mStagingDeviceMemory = nullptr;
    vk::MemoryRequirements mStagingMemoryRequirements;

    vk::DeviceSize mBufferSize = 0;
    uint32_t mMemoryTypeIndex = 0;
    uint32_t mStagingMemoryTypeIndex = 0;

    const uint32_t mNoSuitableDeviceMemoryType = std::numeric_limits<uint32_t>::max();
};


#endif //VULKANGAMES2DBASE_CUSTOMVERTEXBUFFER_H