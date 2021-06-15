#ifndef LITTLEENGINE_WINDOW_HPP
#define LITTLEENGINE_WINDOW_HPP


#include <GLFW/glfw3.h>


class Window {
public:
    Window();

    Window(const char* name, int width, int height);

    ~Window();

    void destroy();

    bool open();

private:
    GLFWwindow* window{};
    bool clean{};
};


#endif //LITTLEENGINE_WINDOW_HPP
