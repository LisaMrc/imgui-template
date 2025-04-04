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

glm::mat4 Camera::getViewMatrix() const
{
    return (m_Mode == CameraMode::Trackball) ? m_Trackball.getViewMatrix() : m_Freefly.getViewMatrix();
}

void Camera::moveFront(float t)
{
    if (m_Mode == CameraMode::Trackball)
        m_Trackball.moveFront(t);
    else
        m_Freefly.attachToPiece(m_Freefly.getViewMatrix() * glm::vec4(0.0f, 0.0f, t, 1.0f));
}

void Camera::rotateLeft(float degrees)
{
    if (m_Mode == CameraMode::Trackball)
        m_Trackball.rotateLeft(degrees);
    else
        m_Freefly.rotateLeft(degrees);
}

void Camera::rotateUp(float degrees)
{
    if (m_Mode == CameraMode::Trackball)
        m_Trackball.rotateUp(degrees);
    else
        m_Freefly.rotateUp(degrees);
}

void Camera::handleMouseMotion(float deltaX, float deltaY)
{
    if (m_Mode == CameraMode::Trackball)
        m_Trackball.handleMouseMotion(deltaX, deltaY);
    else
        m_Freefly.rotateLeft(deltaX * 0.1f); // Sensibilité ajustée
}
