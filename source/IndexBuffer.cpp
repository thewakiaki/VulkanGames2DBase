//
// Created by wakiaki on 3/11/26.
//

#include "../include/IndexBuffer.h"

#include "VKCommandBuffer.h"



bool IndexBuffer::SetupBuffers(const std::unique_ptr<VulkanLogicalDevice> &lDevice, const std::unique_ptr<VulkanPhysicalDevice> &pDevice,
                               std::vector<uint16_t> &indices) {

    if (!VKRenderBuffer::SetupBuffers(lDevice, pDevice, indices)) { return false; }

    BindStagingMemory(indices);

    if (!SetupBuffer(lDevice, CustomVKStructs::BufferType::INDEX_BUFFER)) { return false;}

    if (!SetupMemory(pDevice, lDevice)) { return false;}

    mIndexBuffer->bindMemory(*mIndexMemory, 0);

    return true;
}

bool IndexBuffer::CopyStagingData(const std::unique_ptr<VKCommandBuffer> &cmdBuffer,
    const std::unique_ptr<VulkanLogicalDevice> &lDevice) {

    try {
        cmdBuffer->CopyRenderBuffer(mStagingBuffer, mIndexBuffer, mStageCreateInfoSize, lDevice);
        std::cout << "Successfully Copied Staging Buffer to IndexBuffer\n";
        return true;
    }
    catch (const vk::SystemError& err) {
        std::cerr << "Failed to Copy Staging Buffer: " << err.what() << "\n";
        return false;
    }



}

void IndexBuffer::Cleanup() {
    VKRenderBuffer::Cleanup();

    mIndexBuffer.reset();
    mIndexMemory.reset();
}

bool IndexBuffer::SetupBuffer(const std::unique_ptr<VulkanLogicalDevice> &lDevice, CustomVKStructs::BufferType) {
    vk::BufferCreateInfo bufferCreateInfo = SetBufferCreateInfo();

    try {
        mIndexBuffer = std::make_unique<vk::raii::Buffer>(*lDevice->GetLogicalDevice(), bufferCreateInfo);
        std::cout << "Created Index Buffer\n";
        return true;
    }
    catch (const vk::SystemError& err) {
        std::cerr << "Failed to create Index Buffer: " << err.what() << "\n";
        return false;
    }
}

bool IndexBuffer::SetupMemory(const std::unique_ptr<VulkanPhysicalDevice> &pDevice, const std::unique_ptr<VulkanLogicalDevice> &lDevice) {

    mIndexMemoryRequirements = mIndexBuffer->getMemoryRequirements();

    mIndexMemoryTypeIndex = FindMemoryTypeIndex(pDevice, mIndexMemoryRequirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eDeviceLocal);

    if (mIndexMemoryTypeIndex == mNoSuitableDeviceMemoryType) {
        throw std::runtime_error("No suitable memory type");
    }

    vk::MemoryAllocateInfo memoryAllocateInfo = SetMemoryAllocateInfo();

    try {
        mIndexMemory = std::make_unique<vk::raii::DeviceMemory>(*lDevice->GetLogicalDevice(), memoryAllocateInfo);
        std::cout << "Created Index Buffer Memory\n";
        return true;

    }catch (const vk::SystemError& err) {
        std::cerr << "Failed to create memory for Vertex Buffer: " << err.what() << "\n";
        return false;
    }
}

vk::BufferCreateInfo IndexBuffer::SetBufferCreateInfo() {
    vk::BufferCreateInfo bufferCreateInfo;

    bufferCreateInfo.setSize(mIndexBufferSize);
    bufferCreateInfo.setUsage(vk::BufferUsageFlagBits::eIndexBuffer | vk::BufferUsageFlagBits::eTransferDst);
    bufferCreateInfo.setSharingMode(vk::SharingMode::eExclusive);



    return bufferCreateInfo;
}

vk::MemoryAllocateInfo IndexBuffer::SetMemoryAllocateInfo() const {
    vk::MemoryAllocateInfo memoryAllocateInfo;
    memoryAllocateInfo.setAllocationSize(mIndexMemoryRequirements.size);
    memoryAllocateInfo.setMemoryTypeIndex(mIndexMemoryTypeIndex);

    return memoryAllocateInfo;
}
