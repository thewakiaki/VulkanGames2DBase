//
// Created by wakiaki on 3/11/26.
//

#include "../include/RenderBuffer.h"
#include "CustomLD.h"
#include "CustomPD.h"

bool RenderBuffer::SetupBuffers(const std::unique_ptr<CustomLD>& lDevice, const std::unique_ptr<CustomPD>& pDevice, std::vector<CustomVKStructs::Vertex>& vertices) {

    mVertexBufferSize = sizeof(vertices[0]) * vertices.size();

    if (!RenderBuffer::SetupBuffer(lDevice, CustomVKStructs::BufferType::VERTEX_BUFFER)) { return false; }

    if (!RenderBuffer::SetupMemory(pDevice, lDevice)) { return false; }

    return true;
}

bool RenderBuffer::SetupBuffers(const std::unique_ptr<CustomLD> &lDevice, const std::unique_ptr<CustomPD> &pDevice,
    std::vector<uint16_t> &indices) {

    mIndexBufferSize = sizeof(indices[0]) * indices.size();

    if (!RenderBuffer::SetupBuffer(lDevice, CustomVKStructs::BufferType::INDEX_BUFFER)) { return false; }

    if (!RenderBuffer::SetupMemory(pDevice, lDevice)) { return false; }

    return true;
}

void RenderBuffer::Cleanup() {
    mStagingDeviceMemory.reset();
    mStagingBuffer.reset();
}

bool RenderBuffer::SetupBuffer(const std::unique_ptr<CustomLD>& lDevice, CustomVKStructs::BufferType bufferType) {

    try {
        vk::BufferCreateInfo stagingCreateInfo;
        if (bufferType == CustomVKStructs::BufferType::VERTEX_BUFFER) {
            stagingCreateInfo = SetStageVertexCreateInfo();
        }
        else {
            stagingCreateInfo = SetStageIndexCreateInfo();
        }

        mStagingBuffer = std::make_unique<vk::raii::Buffer>(*lDevice->GetLogicalDevice(), stagingCreateInfo);

        std::cout << "Staging buffer created" << std::endl;
        return true;
    }
    catch (const vk::SystemError& err) {
        std::cerr << "Failed to setup staging buffer Error: " << err.what() << "\n";
        return false;
    }
}

bool RenderBuffer::SetupMemory(const std::unique_ptr<CustomPD> &pDevice, const std::unique_ptr<CustomLD>& lDevice) {

    mStagingMemoryRequirements = mStagingBuffer->getMemoryRequirements();

    mStagingMemoryTypeIndex = FindMemoryTypeIndex(pDevice, mStagingMemoryRequirements.memoryTypeBits,
                                        vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);

    if (mStagingMemoryTypeIndex == mNoSuitableDeviceMemoryType) {
        throw std::runtime_error("No suitable memory type");
    }

    vk::MemoryAllocateInfo memoryAllocateInfo = RenderBuffer::SetMemoryAllocateInfo();

    try {
        mStagingDeviceMemory = std::make_unique<vk::raii::DeviceMemory>(*lDevice->GetLogicalDevice(), memoryAllocateInfo);
        std::cout << "Staging device memory allocated\n";
        return true;
    }
    catch (const vk::SystemError& err) {
        std::cerr << "Failed to setup staging memory allocation Error: " << err.what() << "\n";
        return false;
    }
}

void RenderBuffer::BindStagingMemory(const std::vector<CustomVKStructs::Vertex>& vertices) const {
    mStagingBuffer->bindMemory(*mStagingDeviceMemory, 0);
    void* stagingData = mStagingDeviceMemory->mapMemory(0, mStageCreateInfoSize);
    memcpy(stagingData, vertices.data(), mStageCreateInfoSize);
    mStagingDeviceMemory->unmapMemory();
}

void RenderBuffer::BindStagingMemory(const std::vector<uint16_t> &indices) const {

    mStagingBuffer->bindMemory(*mStagingDeviceMemory, 0);
    void* stagingData = mStagingDeviceMemory->mapMemory(0, mStageCreateInfoSize);
    memcpy(stagingData, indices.data(), mStageCreateInfoSize);
    mStagingDeviceMemory->unmapMemory();
}

vk::BufferCreateInfo RenderBuffer::SetStageVertexCreateInfo() {
    vk::BufferCreateInfo bufferCreateInfo;
    bufferCreateInfo.setSize(mVertexBufferSize);
    bufferCreateInfo.setUsage(vk::BufferUsageFlagBits::eTransferSrc);
    bufferCreateInfo.setSharingMode(vk::SharingMode::eExclusive);

    mStageCreateInfoSize = bufferCreateInfo.size;

    return bufferCreateInfo;
}

vk::BufferCreateInfo RenderBuffer::SetStageIndexCreateInfo() {
    vk::BufferCreateInfo bufferCreateInfo;
    bufferCreateInfo.setSize(mIndexBufferSize);
    bufferCreateInfo.setUsage(vk::BufferUsageFlagBits::eTransferSrc);
    bufferCreateInfo.setSharingMode(vk::SharingMode::eExclusive);

    mStageCreateInfoSize = bufferCreateInfo.size;

    return bufferCreateInfo;
}

uint32_t RenderBuffer::FindMemoryTypeIndex(const std::unique_ptr<CustomPD>& pDevice, const uint32_t &typeFilter, vk::MemoryPropertyFlags properties) const {

    const vk::PhysicalDeviceMemoryProperties memoryProperties = pDevice->GetPhysicalDevice()->getMemoryProperties();

    for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++) {
        if (typeFilter & (1 << i) && (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    return mNoSuitableDeviceMemoryType;
}



vk::BufferCreateInfo RenderBuffer::SetBufferCreateInfo() {
    vk::BufferCreateInfo bufferCreateInfo;
    bufferCreateInfo.setSize(mVertexBufferSize);
    bufferCreateInfo.setUsage(vk::BufferUsageFlagBits::eTransferSrc);
    bufferCreateInfo.setSharingMode(vk::SharingMode::eExclusive);

    mStageCreateInfoSize = bufferCreateInfo.size;

    return bufferCreateInfo;
}

vk::MemoryAllocateInfo RenderBuffer::SetMemoryAllocateInfo() const {
    vk::MemoryAllocateInfo memoryAllocateInfo;
    memoryAllocateInfo.setAllocationSize(mStagingMemoryRequirements.size);
    memoryAllocateInfo.setMemoryTypeIndex(mStagingMemoryTypeIndex);


    return memoryAllocateInfo;
}
