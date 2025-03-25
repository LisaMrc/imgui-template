#include "../include/Camera.hpp"
#include <iostream>

Camera::Camera()
    : m_Mode(CameraMode::Trackball), m_Trackball(), m_Freefly() {}

void Camera::setMode(CameraMode mode)
{
    if (m_Mode == mode)
        return;

    m_Mode = mode;

    if (m_Mode == CameraMode::Trackball)
    {
        m_Trackball = TrackballCamera();
        std::cout << "Mode Trackball activé" << std::endl;
    }
    else
    {
        m_Freefly = FreeflyCamera();
        std::cout << "Mode Freefly activé" << std::endl;
    }
}