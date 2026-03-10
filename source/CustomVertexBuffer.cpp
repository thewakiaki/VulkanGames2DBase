//
// Created by wakiaki on 3/10/26.
//

#include <utility>

#include "../include/CustomVertexBuffer.h"

bool CustomVertexBuffer::SetupVertexBuffer(const std::unique_ptr<CustomPD> &pDevice, const std::unique_ptr<CustomLD>& lDevice, const std::vector<CustomVKStructs::Vertex>& vertices) {

    if (!SetupBuffer(lDevice, vertices)) { return false; }

    if (!SetupMemory(pDevice, lDevice)) { return false; }

    mBuffer->bindMemory(*mDeviceMemory, 0);

    FillBuffer(vertices);

    return true;
}

void CustomVertexBuffer::Cleanup() {
    mBuffer.reset();
    mDeviceMemory.reset();
}

vk::BufferCreateInfo CustomVertexBuffer::CreateBufferInfo(const std::unique_ptr<CustomLD>& lDevice, const std::vector<CustomVKStructs::Vertex>& vertices) {
    vk::BufferCreateInfo bufferCreateInfo;

    bufferCreateInfo.setSize(sizeof(vertices[0]) * vertices.size());
    bufferCreateInfo.setUsage(vk::BufferUsageFlagBits::eVertexBuffer);
    bufferCreateInfo.setSharingMode(vk::SharingMode::eExclusive);

    return bufferCreateInfo;
}

bool CustomVertexBuffer::SetupBuffer(const std::unique_ptr<CustomLD> &lDevice, const std::vector<CustomVKStructs::Vertex> &vertices) {

    try {
        vk::BufferCreateInfo bufferCreateInfo = CreateBufferInfo(lDevice, vertices);
        mBuffer = std::make_unique<vk::raii::Buffer>(*lDevice->GetLogicalDevice(), bufferCreateInfo);
        mMemoryRequirements = mBuffer->getMemoryRequirements();
        mBufferSize = bufferCreateInfo.size;
        return true;
    }
    catch (const vk::SystemError& err) {

        std::cerr << "Failed to Setup Vertex Buffer Error: " << err.what() << std::endl;
        return false;
    }
}

bool CustomVertexBuffer::SetupMemory(const std::unique_ptr<CustomPD>& pDevice, const std::unique_ptr<CustomLD>& lDevice) {

    mMemoryTypeIndex = FindDeviceMemoryType(pDevice, mMemoryRequirements.memoryTypeBits,
                                     vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);

    if (mMemoryTypeIndex == mNoSuitableDeviceMemoryType) { throw std::runtime_error("failed to find suitable memory type!"); }

    vk::MemoryAllocateInfo memoryAllocateInfo = CreateDeviceMemInfo(mMemoryTypeIndex);

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

uint32_t CustomVertexBuffer::FindDeviceMemoryType(const std::unique_ptr<CustomPD> &pDevice, uint32_t typeFilter, vk::MemoryPropertyFlags properties) {

    vk::PhysicalDeviceMemoryProperties memoryProperties = pDevice->GetPhysicalDevice()->getMemoryProperties();

    for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++) {
        if (typeFilter & (1 << i) && (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    return std::numeric_limits<uint32_t>::max();
}

vk::MemoryAllocateInfo CustomVertexBuffer::CreateDeviceMemInfo(const uint32_t& memTypeIndex) {
    vk::MemoryAllocateInfo memoryAllocateInfo;

    memoryAllocateInfo.setAllocationSize(mMemoryRequirements.size);
    memoryAllocateInfo.setMemoryTypeIndex(memTypeIndex);
    return memoryAllocateInfo;
}

void CustomVertexBuffer::FillBuffer(const std::vector<CustomVKStructs::Vertex>& vertices) {
    void* data = mDeviceMemory->mapMemory(0, mBufferSize);
    memcpy(data, vertices.data(), mBufferSize);
    mDeviceMemory->unmapMemory();
}
