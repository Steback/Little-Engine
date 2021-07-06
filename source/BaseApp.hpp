#ifndef LITTLEVULKANENGINE_BASEAPP_HPP
#define LITTLEVULKANENGINE_BASEAPP_HPP


#include <memory>


namespace lve {

    class Window;

    class BaseApp {
    public:
        BaseApp();

        ~BaseApp();

        void run();

    private:
        std::shared_ptr<Window> window;
    };

} // namespace lve


#endif //LITTLEVULKANENGINE_BASEAPP_HPP
