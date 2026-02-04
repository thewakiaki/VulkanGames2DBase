//
// Created by wakiaki on 1/14/26.
//
#ifndef VULKANGAMES2DBASE_CUSTOMVKSTRUCTS_H
#define VULKANGAMES2DBASE_CUSTOMVKSTRUCTS_H
#include <vulkan/vulkan_core.h>
#include <vulkan/vulkan_raii.hpp>



namespace CustomVKStructs {

    struct PhysicalDeviceScore {
        PhysicalDeviceScore(int scr, vk::raii::PhysicalDevice pd = VK_NULL_HANDLE)
                : score(scr), physical_device(pd) {}

        int score;
        vk::raii::PhysicalDevice physical_device = VK_NULL_HANDLE;
    };

}

#endif //VULKANGAMES2DBASE_CUSTOMVKSTRUCTS_H
