#include "../include/Input.hpp"

bool isKeyPressed(int key) {
    return glfwGetKey(glfwGetCurrentContext(), key) == GLFW_PRESS;
}

bool isMouseMoving() {
    static double lastX = 0, lastY = 0;
    double x, y;
    glfwGetCursorPos(glfwGetCurrentContext(), &x, &y);
    bool moving = (x != lastX || y != lastY);
    lastX = x;
    lastY = y;
    return moving;
}

float getMouseDX() {
    static double lastX = 0;
    double x;
    glfwGetCursorPos(glfwGetCurrentContext(), &x, nullptr);
    float dx = static_cast<float>(x - lastX);
    lastX = x;
    return dx;
}

float getMouseDY() {
    static double lastY = 0;
    double y;
    glfwGetCursorPos(glfwGetCurrentContext(), nullptr, &y);
    float dy = static_cast<float>(y - lastY);
    lastY = y;
    return dy;
}
