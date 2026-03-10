//
// Created by wakiaki on 3/10/26.
//

#include "../include/CustomVertexBuffer.h"

bool CustomVertexBuffer::SetupBuffers(const std::unique_ptr<CustomPD> &pDevice, const std::unique_ptr<CustomLD>& lDevice, const std::vector<CustomVKStructs::Vertex>& vertices) {

    mBufferSize = sizeof(vertices[0]) * vertices.size();

    if (!SetupStagingVertexBuffer(pDevice, lDevice, vertices)) { return false; }

    if (!SetupVertexBuffer(pDevice, lDevice, vertices)) { return false; }

    return true;
}

bool CustomVertexBuffer::SetupVertexBuffer(const std::unique_ptr<CustomPD> &pDevice, const std::unique_ptr<CustomLD>& lDevice, const std::vector<CustomVKStructs::Vertex>& vertices) {

    if (!SetupBuffer(lDevice, vertices)) { return false; }

    if (!SetupMemory(pDevice, lDevice)) { return false; }

    mBuffer->bindMemory(*mDeviceMemory, 0);

    return true;
}

bool CustomVertexBuffer::SetupStagingVertexBuffer(const std::unique_ptr<CustomPD> &pDevice,
    const std::unique_ptr<CustomLD> &lDevice, const std::vector<CustomVKStructs::Vertex> &vertices) {

    if (!SetupStagingBuffer(lDevice, vertices)) { return false; }

    if (!SetupStagingMemory(pDevice, lDevice)) { return false; }

    mStagingBuffer->bindMemory(*mStagingDeviceMemory, 0);

    FillBuffer(vertices);

    return true;

}

void CustomVertexBuffer::Cleanup() {
    mBuffer.reset();
    mDeviceMemory.reset();
}

vk::BufferCreateInfo CustomVertexBuffer::CreateBufferInfo(const std::unique_ptr<CustomLD>& lDevice, const std::vector<CustomVKStructs::Vertex>& vertices, BufferType bufferType) {

    vk::BufferCreateInfo bufferCreateInfo;

    bufferCreateInfo.setSize(mBufferSize);
    bufferCreateInfo.setSharingMode(vk::SharingMode::eExclusive);

    switch (bufferType) {
        case BufferType::STAGING_BUFFER:
            bufferCreateInfo.setUsage(vk::BufferUsageFlagBits::eTransferSrc);
            break;
        case BufferType::VERTEX_BUFFER:
            bufferCreateInfo.setUsage(vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst);
            break;
    }

    return bufferCreateInfo;
}

bool CustomVertexBuffer::SetupBuffer(const std::unique_ptr<CustomLD> &lDevice, const std::vector<CustomVKStructs::Vertex> &vertices) {

    try {
        vk::BufferCreateInfo bufferCreateInfo = CreateBufferInfo(lDevice, vertices, BufferType::VERTEX_BUFFER);
        mBuffer = std::make_unique<vk::raii::Buffer>(*lDevice->GetLogicalDevice(), bufferCreateInfo);
        mMemoryRequirements = mBuffer->getMemoryRequirements();
        return true;
    }
    catch (const vk::SystemError& err) {

        std::cerr << "Failed to Setup Vertex Buffer Error: " << err.what() << std::endl;
        return false;
    }
}

bool CustomVertexBuffer::SetupStagingBuffer(const std::unique_ptr<CustomLD> &lDevice,
    const std::vector<CustomVKStructs::Vertex> &vertices) {

    mBufferSize = sizeof(vertices[0]) * vertices.size();

    try {
        vk::BufferCreateInfo bufferCreateInfo = CreateBufferInfo(lDevice, vertices, BufferType::STAGING_BUFFER);
        mStagingBuffer = std::make_unique<vk::raii::Buffer>(*lDevice->GetLogicalDevice(), bufferCreateInfo);
        mStagingMemoryRequirements = mStagingBuffer->getMemoryRequirements();
        return true;
    }
    catch (const vk::SystemError& err) {

        std::cerr << "Failed to Setup Vertex Buffer Error: " << err.what() << std::endl;
        return false;
    }
}

bool CustomVertexBuffer::SetupMemory(const std::unique_ptr<CustomPD>& pDevice, const std::unique_ptr<CustomLD>& lDevice) {

    mMemoryTypeIndex = FindDeviceMemoryType(pDevice, mMemoryRequirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eDeviceLocal);

    if (mMemoryTypeIndex == mNoSuitableDeviceMemoryType) { throw std::runtime_error("failed to find suitable memory type!"); }

    vk::MemoryAllocateInfo memoryAllocateInfo = CreateDeviceMemInfo(mMemoryTypeIndex, BufferType::VERTEX_BUFFER);

    try {
        mDeviceMemory = std::make_unique<vk::raii::DeviceMemory>(*lDevice->GetLogicalDevice(), memoryAllocateInfo);
        std::cout << "Device Memory Setup Type: " << mMemoryTypeIndex << std::endl;
        return true;
    }
    catch (const vk::SystemError& err) {
        std::cerr << "Failed to Setup Memory Allocation Error: " << err.what() << std::endl;
        return false;
    }


}

bool CustomVertexBuffer::SetupStagingMemory(const std::unique_ptr<CustomPD> &pDevice,
    const std::unique_ptr<CustomLD> &lDevice) {

    mStagingMemoryTypeIndex = FindDeviceMemoryType(pDevice, mStagingMemoryRequirements.memoryTypeBits,
        vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);

    if (mStagingMemoryTypeIndex == mNoSuitableDeviceMemoryType) { throw std::runtime_error("failed to find suitable memory type!"); }

    vk::MemoryAllocateInfo memoryAllocateInfo = CreateDeviceMemInfo(mStagingMemoryTypeIndex, BufferType::STAGING_BUFFER);

    try {
        mStagingDeviceMemory = std::make_unique<vk::raii::DeviceMemory>(*lDevice->GetLogicalDevice(), memoryAllocateInfo);
        std::cout << "Device Memory Setup Type: " << mMemoryTypeIndex << std::endl;
        return true;
    }
    catch (const vk::SystemError& err) {
        std::cerr << "Failed to Setup Memory Allocation Error: " << err.what() << std::endl;
        return false;
    }
}

uint32_t CustomVertexBuffer::FindDeviceMemoryType(const std::unique_ptr<CustomPD> &pDevice, uint32_t typeFilter, vk::MemoryPropertyFlags properties) {

    vk::PhysicalDeviceMemoryProperties memoryProperties = pDevice->GetPhysicalDevice()->getMemoryProperties();

    for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++) {
        if (typeFilter & (1 << i) && (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    return std::numeric_limits<uint32_t>::max();
}

vk::MemoryAllocateInfo CustomVertexBuffer::CreateDeviceMemInfo(const uint32_t& memTypeIndex, BufferType bufferType) {
    vk::MemoryAllocateInfo memoryAllocateInfo;


    memoryAllocateInfo.setMemoryTypeIndex(memTypeIndex);

    switch (bufferType) {
        case BufferType::STAGING_BUFFER:
            memoryAllocateInfo.setAllocationSize(mStagingMemoryRequirements.size);
            break;
        case BufferType::VERTEX_BUFFER:
            memoryAllocateInfo.setAllocationSize(mMemoryRequirements.size);
            break;
    }

    return memoryAllocateInfo;
}

void CustomVertexBuffer::FillBuffer(const std::vector<CustomVKStructs::Vertex>& vertices) {

    void* data = mStagingDeviceMemory->mapMemory(0, mBufferSize);
    memcpy(data, vertices.data(), mBufferSize);
    mStagingDeviceMemory->unmapMemory();

}
