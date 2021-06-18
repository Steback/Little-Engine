#ifndef LITTLEENGINE_BASE_HPP
#define LITTLEENGINE_BASE_HPP


#include <memory>


class Window;
class Device;
class GraphicsPipeline;

class Base {
public:
    Base();

    ~Base();

    void setup();

    void update();

    void destroy();

    void loop();

private:
    std::shared_ptr<Window> window;
    std::shared_ptr<Device> device;
    std::shared_ptr<GraphicsPipeline> pipeline;
};


#endif //LITTLEENGINE_BASE_HPP
