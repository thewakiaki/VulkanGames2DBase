//
// Created by wakiaki on 3/11/26.
//

#include "../include/VKRenderBuffer.h"
#include "VulkanLogicalDevice.h"
#include "VulkanPhysicalDevice.h"

bool VKRenderBuffer::SetupBuffers(const std::unique_ptr<VulkanLogicalDevice>& lDevice, const std::unique_ptr<VulkanPhysicalDevice>& pDevice, std::vector<CustomVKStructs::Vertex>& vertices) {

    mVertexBufferSize = sizeof(CustomVKStructs::Vertex) * vertices.size();

    if (!VKRenderBuffer::SetupBuffer(lDevice, CustomVKStructs::BufferType::VERTEX_BUFFER)) { return false; }

    if (!VKRenderBuffer::SetupMemory(pDevice, lDevice)) { return false; }

    return true;
}

bool VKRenderBuffer::SetupBuffers(const std::unique_ptr<VulkanLogicalDevice> &lDevice, const std::unique_ptr<VulkanPhysicalDevice> &pDevice,
    std::vector<uint16_t> &indices) {

    mIndexBufferSize = sizeof(indices[0]) * indices.size();

    if (!VKRenderBuffer::SetupBuffer(lDevice, CustomVKStructs::BufferType::INDEX_BUFFER)) { return false; }

    if (!VKRenderBuffer::SetupMemory(pDevice, lDevice)) { return false; }

    return true;
}

void VKRenderBuffer::Cleanup() {
    mStagingDeviceMemory.reset();
    mStagingBuffer.reset();
}

bool VKRenderBuffer::SetupBuffer(const std::unique_ptr<VulkanLogicalDevice>& lDevice, CustomVKStructs::BufferType bufferType) {

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

bool VKRenderBuffer::SetupMemory(const std::unique_ptr<VulkanPhysicalDevice> &pDevice, const std::unique_ptr<VulkanLogicalDevice>& lDevice) {

    mStagingMemoryRequirements = mStagingBuffer->getMemoryRequirements();

    std::cout << "Staging mem reqs.size: " << mStagingMemoryRequirements.size << "\n";
    std::cout << "mStageCreateInfoSize: " << mStageCreateInfoSize << "\n";

    mStagingMemoryTypeIndex = FindMemoryTypeIndex(pDevice, mStagingMemoryRequirements.memoryTypeBits,
                                        vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);

    if (mStagingMemoryTypeIndex == mNoSuitableDeviceMemoryType) {
        throw std::runtime_error("No suitable memory type");
    }

    vk::MemoryAllocateInfo memoryAllocateInfo = VKRenderBuffer::SetMemoryAllocateInfo();

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

void VKRenderBuffer::BindStagingMemory(const std::vector<CustomVKStructs::Vertex>& vertices) {
    std::cout << "mStageCreateInfoSize: " << mStageCreateInfoSize << " (should be 60)\n";
    std::cout << "mVertexBufferSize: " << mVertexBufferSize << "\n";

    mStagingBuffer->bindMemory(*mStagingDeviceMemory, 0);
    void* stagingData = mStagingDeviceMemory->mapMemory(0, mStagingBuffer->getMemoryRequirements().size);
    memcpy(stagingData, vertices.data(), mVertexBufferSize);
    mStagingDeviceMemory->unmapMemory();
}

void VKRenderBuffer::BindStagingMemory(const std::vector<uint16_t> &indices) const {
    mStagingBuffer->bindMemory(*mStagingDeviceMemory, 0);
    void* stagingData = mStagingDeviceMemory->mapMemory(0, mStageCreateInfoSize);
    memcpy(stagingData, indices.data(), mIndexBufferSize);
    mStagingDeviceMemory->unmapMemory();
}

vk::BufferCreateInfo VKRenderBuffer::SetStageVertexCreateInfo() {
    vk::BufferCreateInfo bufferCreateInfo;
    bufferCreateInfo.setSize(mVertexBufferSize);
    bufferCreateInfo.setUsage(vk::BufferUsageFlagBits::eTransferSrc);
    bufferCreateInfo.setSharingMode(vk::SharingMode::eExclusive);

    mStageCreateInfoSize = bufferCreateInfo.size;

    return bufferCreateInfo;
}

vk::BufferCreateInfo VKRenderBuffer::SetStageIndexCreateInfo() {
    vk::BufferCreateInfo bufferCreateInfo;
    bufferCreateInfo.setSize(mIndexBufferSize);
    bufferCreateInfo.setUsage(vk::BufferUsageFlagBits::eTransferSrc);
    bufferCreateInfo.setSharingMode(vk::SharingMode::eExclusive);

    mStageCreateInfoSize = bufferCreateInfo.size;

    return bufferCreateInfo;
}

uint32_t VKRenderBuffer::FindMemoryTypeIndex(const std::unique_ptr<VulkanPhysicalDevice>& pDevice, const uint32_t &typeFilter, vk::MemoryPropertyFlags properties) const {

    const vk::PhysicalDeviceMemoryProperties memoryProperties = pDevice->GetPhysicalDevice()->getMemoryProperties();

    for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++) {
        if (typeFilter & (1 << i) && (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    return mNoSuitableDeviceMemoryType;
}



vk::BufferCreateInfo VKRenderBuffer::SetBufferCreateInfo() {
    vk::BufferCreateInfo bufferCreateInfo;
    bufferCreateInfo.setSize(mVertexBufferSize);
    bufferCreateInfo.setUsage(vk::BufferUsageFlagBits::eTransferSrc);
    bufferCreateInfo.setSharingMode(vk::SharingMode::eExclusive);

    mStageCreateInfoSize = bufferCreateInfo.size;

    return bufferCreateInfo;
}

vk::MemoryAllocateInfo VKRenderBuffer::SetMemoryAllocateInfo() const {
    vk::MemoryAllocateInfo memoryAllocateInfo;
    memoryAllocateInfo.setAllocationSize(mStagingMemoryRequirements.size);
    memoryAllocateInfo.setMemoryTypeIndex(mStagingMemoryTypeIndex);


    return memoryAllocateInfo;
}
