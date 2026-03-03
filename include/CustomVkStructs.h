//
// Created by wakiaki on 1/14/26.
//
#ifndef VULKANGAMES2DBASE_CUSTOMVKSTRUCTS_H
#define VULKANGAMES2DBASE_CUSTOMVKSTRUCTS_H



namespace CustomVKStructs {

    struct PhysicalDeviceScore {
        PhysicalDeviceScore() = default;
        PhysicalDeviceScore(int scr, std::unique_ptr<vk::raii::PhysicalDevice> device) : score(scr), physical_device{std::move(device)} {}

        void SetDevice(std::unique_ptr<vk::raii::PhysicalDevice> device) { physical_device = std::move(device);}

        int score = 0;
        std::unique_ptr<vk::raii::PhysicalDevice> physical_device;
    };

    enum class RequiredVkFamilies{
        Graphics = 0,
        Present = 1,
    };

    struct VkQueueFamilies{
        VkQueueFamilies(RequiredVkFamilies type, const char* name = "", uint32_t index = FAMILY_NOT_FOUND, bool found = false) : familyType(type), familyName(name), familyIndex(index), familyFound(found) {}

        RequiredVkFamilies familyType;
        const char* familyName;
        uint32_t familyIndex;
        bool familyFound = false;
    };

    constexpr size_t MAX_FRAMES_IN_FLIGHT = 2;

}

#endif //VULKANGAMES2DBASE_CUSTOMVKSTRUCTS_H
