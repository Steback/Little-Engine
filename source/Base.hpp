#ifndef LITTLEENGINE_BASE_HPP
#define LITTLEENGINE_BASE_HPP


#include <memory>


class Window;


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
};


#endif //LITTLEENGINE_BASE_HPP
