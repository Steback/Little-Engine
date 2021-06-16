#ifndef LITTLEENGINE_BASE_HPP
#define LITTLEENGINE_BASE_HPP


#include <memory>


class Window;
class Device;
class Pipeline;

class Base {
public:
    Base();

    ~Base();

    void setup();

    void update();

    void destroy();

    void loop();

private:
    std::shared_ptr<Window> mainWindow;
    std::shared_ptr<Device> device;
    std::shared_ptr<Pipeline> pipeline;
};


#endif //LITTLEENGINE_BASE_HPP
