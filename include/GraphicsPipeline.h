#ifndef GRAPHICS_PIPELINE_H
#define GRAPHICS_PIPELINE_H

#include "CustomSM.h"
#include "CustomLD.h"
#include <memory>
#include <vulkan/vulkan_raii.hpp>


class GraphicsPipeline{
public:
    GraphicsPipeline(const CustomLD& lDevice) : mLogicalDevice(lDevice){}

    bool SetupShaders();
    bool CreatePipeline();

private:

    std::unique_ptr<vk::raii::Pipeline> mPipeline;

    const CustomLD& mLogicalDevice;

    std::unique_ptr<CustomSM> mVertShader;
    std::unique_ptr<CustomSM> mFragShader;

};

#endif // GRAPHICS_PIPELINE_H
