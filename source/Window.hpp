#ifndef LITTLEENGINE_WINDOW_HPP
#define LITTLEENGINE_WINDOW_HPP


#include <GLFW/glfw3.h>


class Window {
public:
    Window();

    Window(const Window &) = delete;

    Window(const char* name, int width, int height);

    Window &operator=(const Window &) = delete;

    ~Window();

    void destroy();

    bool open();

private:
    GLFWwindow* window{};
};


#endif //LITTLEENGINE_WINDOW_HPP
