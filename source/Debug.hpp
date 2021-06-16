#ifndef LITTLEENGINE_DEBUG_HPP
#define LITTLEENGINE_DEBUG_HPP


PFN_vkCreateDebugUtilsMessengerEXT  pfnVkCreateDebugUtilsMessengerEXT;
PFN_vkDestroyDebugUtilsMessengerEXT pfnVkDestroyDebugUtilsMessengerEXT;

VKAPI_ATTR VkResult VKAPI_CALL vkCreateDebugUtilsMessengerEXT(VkInstance instance,
                                                              const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                                              const VkAllocationCallbacks* pAllocator,
                                                              VkDebugUtilsMessengerEXT* pMessenger) {
    return pfnVkCreateDebugUtilsMessengerEXT(instance, pCreateInfo, pAllocator, pMessenger);
}

VKAPI_ATTR void VKAPI_CALL vkDestroyDebugUtilsMessengerEXT(VkInstance instance,
                                                           VkDebugUtilsMessengerEXT messenger,
                                                           const VkAllocationCallbacks* pAllocator) {
    return pfnVkDestroyDebugUtilsMessengerEXT(instance, messenger, pAllocator);
}

VKAPI_ATTR VkBool32 VKAPI_CALL debugMessageFunc(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                VkDebugUtilsMessageTypeFlagsEXT messageTypes,
                                                const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                void * /*pUserData*/ ) {
    std::string message;

    message += vk::to_string( static_cast<vk::DebugUtilsMessageSeverityFlagBitsEXT>( messageSeverity ) ) + ": " +
               vk::to_string( static_cast<vk::DebugUtilsMessageTypeFlagsEXT>( messageTypes ) ) + ":\n";
    message += std::string( "\t" ) + "messageIDName   = <" + pCallbackData->pMessageIdName + ">\n";
    message += std::string( "\t" ) + "messageIdNumber = " + std::to_string( pCallbackData->messageIdNumber ) + "\n";
    message += std::string( "\t" ) + "message         = <" + pCallbackData->pMessage + ">\n";

    if (0 < pCallbackData->queueLabelCount) {
        message += std::string( "\t" ) + "Queue Labels:\n";

        for (uint8_t i = 0; i < pCallbackData->queueLabelCount; i++)
            message += std::string( "\t\t" ) + "labelName = <" + pCallbackData->pQueueLabels[i].pLabelName + ">\n";
    }

    if (0 < pCallbackData->cmdBufLabelCount) {
        message += std::string("\t") + "CommandBuffer Labels:\n";

        for (uint8_t i = 0; i < pCallbackData->cmdBufLabelCount; i++)
            message += std::string( "\t\t" ) + "labelName = <" + pCallbackData->pCmdBufLabels[i].pLabelName + ">\n";
    }

    if (0 < pCallbackData->objectCount) {

        for (uint8_t i = 0; i < pCallbackData->objectCount; i++) {
            message += std::string( "\t" ) + "Object " + std::to_string( i ) + "\n";
            message += std::string( "\t\t" ) + "objectType   = " +
                       vk::to_string( static_cast<vk::ObjectType>( pCallbackData->pObjects[i].objectType ) ) + "\n";
            message += std::string( "\t\t" ) + "objectHandle = " + std::to_string( pCallbackData->pObjects[i].objectHandle ) + "\n";

            if (pCallbackData->pObjects[i].pObjectName)
                message += std::string( "\t\t" ) + "objectName   = <" + pCallbackData->pObjects[i].pObjectName + ">\n";
        }
    }

    spdlog::error("{}", message);

    return false;
}


#endif //LITTLEENGINE_DEBUG_HPP
