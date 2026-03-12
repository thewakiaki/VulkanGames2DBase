//
// Created by wakiaki on 3/11/26.
//

#include "../include/VertexBuffer.h"

#include "CommandBuffer.h"

bool VertexBuffer::SetupBuffers(const std::unique_ptr<CustomLD> &lDevice, const std::unique_ptr<CustomPD> &pDevice, std::vector<CustomVKStructs::Vertex>& vertices) {
    std::cout << "sizeof(Vertex): " << sizeof(CustomVKStructs::Vertex) << std::endl;      // 32?
    std::cout << "mVertices.size(): " << vertices.size() << std::endl;  // 3?
    std::cout << "Copy size: " << (vertices.size() * sizeof(CustomVKStructs::Vertex)) << std::endl;

    if (!RenderBuffer::SetupBuffers(lDevice, pDevice, vertices)) { return false; }

    BindStagingMemory(vertices);

    if (!SetupBuffer(lDevice, CustomVKStructs::BufferType::VERTEX_BUFFER)) { return false; }

    if (!SetupMemory(pDevice, lDevice)) { return false; }

    mVertexBuffer->bindMemory(*mVertexMemory, 0);

    return true;
}

bool VertexBuffer::CopyStagingData(const std::unique_ptr<CmdBuffer> &cmdBuffer, const std::unique_ptr<CustomLD>& lDevice) {

    try {
        cmdBuffer->CopyRenderBuffer(mStagingBuffer, mVertexBuffer, mStageCreateInfoSize, lDevice);
        std::cout << "Successfully Copied Staging Buffer to VertexBuffer\n";
        return true;
    }
    catch (const vk::SystemError& err) {
        std::cerr << "Failed to Copy Staging Buffer: " << err.what() << "\n";
        return false;
    }

}

void VertexBuffer::Cleanup() {
    RenderBuffer::Cleanup();

    mVertexBuffer.reset();
    mVertexMemory.reset();
}

bool VertexBuffer::SetupBuffer(const std::unique_ptr<CustomLD> &lDevice, CustomVKStructs::BufferType bufferType) {
    vk::BufferCreateInfo bufferCreateInfo = SetBufferCreateInfo();

    try {
        mVertexBuffer = std::make_unique<vk::raii::Buffer>(*lDevice->GetLogicalDevice(), bufferCreateInfo);
        std::cout << "Created Vertex Buffer\n";
        return true;
    }
    catch (const vk::SystemError& err) {
        std::cerr << "Failed to create Vertex Buffer: " << err.what() << "\n";
        return false;
    }

}

bool VertexBuffer::SetupMemory(const std::unique_ptr<CustomPD> &pDevice, const std::unique_ptr<CustomLD> &lDevice) {

    mVertexMemoryRequirements = mVertexBuffer->getMemoryRequirements();

    mVertexMemoryTypeIndex = FindMemoryTypeIndex(pDevice, mVertexMemoryRequirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eDeviceLocal);

    if (mVertexMemoryTypeIndex == mNoSuitableDeviceMemoryType) {
        throw std::runtime_error("No suitable memory type");
    }

    vk::MemoryAllocateInfo memoryAllocateInfo = SetMemoryAllocateInfo();

    try {
        mVertexMemory = std::make_unique<vk::raii::DeviceMemory>(*lDevice->GetLogicalDevice(), memoryAllocateInfo);
        std::cout << "Created Vertex Buffer Memory\n";
        return true;

    }catch (const vk::SystemError& err) {
        std::cerr << "Failed to create memory for Vertex Buffer: " << err.what() << "\n";
        return false;
    }
}

vk::BufferCreateInfo VertexBuffer::SetBufferCreateInfo() {
    vk::BufferCreateInfo bufferCreateInfo;
    bufferCreateInfo.setSize(mVertexBufferSize);
    bufferCreateInfo.setUsage(vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst);
    bufferCreateInfo.setSharingMode(vk::SharingMode::eExclusive);

    return bufferCreateInfo;
}

vk::MemoryAllocateInfo VertexBuffer::SetMemoryAllocateInfo() const {
    vk::MemoryAllocateInfo memoryAllocateInfo;
    memoryAllocateInfo.setAllocationSize(mVertexMemoryRequirements.size);
    memoryAllocateInfo.setMemoryTypeIndex(mVertexMemoryTypeIndex);

    return memoryAllocateInfo;
}
