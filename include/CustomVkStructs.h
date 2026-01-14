//
// Created by wakiaki on 1/14/26.
//
#ifndef VULKANGAMES2DBASE_CUSTOMVKSTRUCTS_H
#define VULKANGAMES2DBASE_CUSTOMVKSTRUCTS_H
#include <vulkan/vulkan_raii.hpp>
#include <optional>


namespace CustomVKStructs {

    struct PhysicalDeviceScore {
        int score;
        std::optional<vk::raii::PhysicalDevice> physical_device;
    };

}

#endif //VULKANGAMES2DBASE_CUSTOMVKSTRUCTS_H