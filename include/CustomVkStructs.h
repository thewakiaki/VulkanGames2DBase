//
// Created by wakiaki on 1/14/26.
//
#ifndef VULKANGAMES2DBASE_CUSTOMVKSTRUCTS_H
#define VULKANGAMES2DBASE_CUSTOMVKSTRUCTS_H
#include <memory>
#include <vulkan/vulkan_core.h>
#include <vulkan/vulkan_raii.hpp>



namespace CustomVKStructs {

    struct PhysicalDeviceScore {
        PhysicalDeviceScore() = default;
        PhysicalDeviceScore(int scr, std::unique_ptr<vk::raii::PhysicalDevice> device) : score(scr), physical_device{std::move(device)} {}

        void SetDevice(std::unique_ptr<vk::raii::PhysicalDevice> device) { physical_device = std::move(device);}

        int score = 0;
        std::unique_ptr<vk::raii::PhysicalDevice> physical_device;
    };
}

#endif //VULKANGAMES2DBASE_CUSTOMVKSTRUCTS_H
