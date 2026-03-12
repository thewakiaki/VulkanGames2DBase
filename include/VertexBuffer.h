//
// Created by wakiaki on 3/11/26.
//

#ifndef VULKANGAMES2DBASE_VERTEXBUFFER_H
#define VULKANGAMES2DBASE_VERTEXBUFFER_H
#include "RenderBuffer.h"

class CustomLD;
class CustomPD;
class CmdBuffer;

class VertexBuffer : public RenderBuffer {

public:
    bool SetupBuffers(const std::unique_ptr<CustomLD>& lDevice, const std::unique_ptr<CustomPD>& pDevice, std::vector<CustomVKStructs::Vertex>& vertices) override;
    bool CopyStagingData(const std::unique_ptr<CmdBuffer> &cmdBuffer, const std::unique_ptr<CustomLD>& lDevice) override;

    [[nodiscard]] const std::unique_ptr<vk::raii::Buffer>& GetBuffer() const override { return mVertexBuffer; }

    void Cleanup() override;

protected:
    bool SetupBuffer(const std::unique_ptr<CustomLD> &lDevice, CustomVKStructs::BufferType bufferType) override;
    bool SetupMemory(const std::unique_ptr<CustomPD> &pDevice, const std::unique_ptr<CustomLD> &lDevice) override;

    vk::BufferCreateInfo SetBufferCreateInfo() override;
    [[nodiscard]] vk::MemoryAllocateInfo SetMemoryAllocateInfo() const override;


private:
    std::unique_ptr<vk::raii::Buffer> mVertexBuffer = nullptr;
    std::unique_ptr<vk::raii::DeviceMemory> mVertexMemory = nullptr;
    vk::MemoryRequirements mVertexMemoryRequirements;

    uint32_t mVertexMemoryTypeIndex = 0;
};


#endif //VULKANGAMES2DBASE_VERTEXBUFFER_H