#ifndef CUSTOM_IV_H
#define CUSTOM_IV_H

#include "CustomSC.h"

class CustomIV{

public:

    bool CreateImageViews(const std::unique_ptr<CustomSC>& swapchain, const std::unique_ptr<CustomLD>& lDevice);


private:

    std::unique_ptr<std::vector<vk::raii::ImageView>> mSwapChainImageViews;

};

#endif // CUSTOM_IV_H
