#ifndef CUSTOM_SURFACE_H
#define CUSTOM_SURFACE_H


class VulkanSurface{
    public :
        bool CreateSurface(const std::unique_ptr<vk::raii::Instance>& vkInstance, GLFWwindow* window);
        void Cleanup();

        void SetCapabilities(const std::unique_ptr<vk::raii::PhysicalDevice>& device) { mSurfaceCapabilities = device->getSurfaceCapabilitiesKHR(*mSurface); }
        void SetFormats(const std::unique_ptr<vk::raii::PhysicalDevice>& device) { mAvailableFormats = device->getSurfaceFormatsKHR(*mSurface); };
        void SetPresentModes(const std::unique_ptr<vk::raii::PhysicalDevice>& device) { mAvailablePresentModes = device->getSurfacePresentModesKHR(*mSurface); }

        bool SetupSurfaceVariables(GLFWwindow* window);

        [[nodiscard]] const vk::SurfaceCapabilitiesKHR& GetCapabilities() const { return mSurfaceCapabilities; }
        [[nodiscard]] const std::vector<vk::SurfaceFormatKHR>& GetAvailableFormats() const { return mAvailableFormats; }
        [[nodiscard]] const std::vector<vk::PresentModeKHR>& GetPresentModes() const { return mAvailablePresentModes; }
        [[nodiscard]] const std::unique_ptr<vk::raii::SurfaceKHR>& GetSurface() const { return mSurface; }

        [[nodiscard]] vk::Extent2D GetChosenExtent() const { return mSurfaceExtent; }
        [[nodiscard]] vk::SurfaceFormatKHR GetChosenFormat() const { return mSurfaceFormat; }
        [[nodiscard]] vk::PresentModeKHR GetChosenPresentMode() const { return mPresentMode; }

    private :
        bool SelectSurfaceFormat();
        bool SelectPresentMode();
        bool SetupSurfaceCapabilities(GLFWwindow* window);

        VkBool32 mDeviceSupported = false;

        std::unique_ptr<vk::raii::SurfaceKHR> mSurface;

        vk::SurfaceCapabilitiesKHR mSurfaceCapabilities;
        std::vector<vk::SurfaceFormatKHR> mAvailableFormats;
        std::vector<vk::PresentModeKHR> mAvailablePresentModes;

        vk::SurfaceFormatKHR mSurfaceFormat;
        vk::PresentModeKHR mPresentMode = vk::PresentModeKHR();
        vk::Extent2D mSurfaceExtent;

        int mSurfaceWidth = 0;
        int mSurfaceHeight = 0;
};

#endif// CUSTOM_SURFACE_H
