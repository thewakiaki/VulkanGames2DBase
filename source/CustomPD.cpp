//
// Created by wakiaki on 1/14/26.
//

#include <utility>

#include "../include/CustomPD.h"


CustomPD::CustomPD(vk::raii::PhysicalDevice device) : mPhysicalDevice(std::move(device)) {
}

void CustomPD::SelectPhysicalDevice(const vk::raii::Instance& vk_instance) {

    std::vector<vk::raii::PhysicalDevice> devices = vk_instance.enumeratePhysicalDevices();

    if (devices.empty()) {
        std::cerr << "No physical device found\n";
        return;
    }

    std::cout << "Physical device found\n";

    int score = 0;

    for (const vk::raii::PhysicalDevice& device : devices) {
        vk::PhysicalDeviceProperties deviceProperties = device.getProperties();
        vk::PhysicalDeviceFeatures deviceFeatures = device.getFeatures();

        if (!DeviceTypeSuitable(device)) {
            std::cerr << "Physical device type not suitable\n";
            continue;
        }

        ScoreDevice(device);
        std::cout << "Device : " << device.getProperties().deviceName << " Scored: " << score << "\n";

        score = 0;
    }

    GetMostSuitableDevice();
}

void CustomPD::ScoreDevice(const vk::raii::PhysicalDevice &device) {

}

void CustomPD::GetMostSuitableDevice() {

}

bool CustomPD::DeviceTypeSuitable(const vk::raii::PhysicalDevice& device) {

    vk::PhysicalDeviceProperties deviceProperties = device.getProperties();
    vk::PhysicalDeviceFeatures deviceFeatures = device.getFeatures();

    return deviceProperties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu;
}
