#pragma once
#include "../include/TrackballCamera.hpp"

// TODO (lisam): short functions : inline ?
void TrackballCamera::moveFront(float delta)
{
    m_fDistance += delta;
}

void TrackballCamera::rotateLeft(float degrees)
{
    m_fAngleY += degrees;
}

void TrackballCamera::rotateUp(float degrees)
{
    m_fAngleX += degrees;
    m_fAngleX = glm::clamp(m_fAngleX, -89.0f, 89.0f);
}

glm::mat4 TrackballCamera::getViewMatrix() const
{
    // Start with identity matrix
    glm::mat4 view = glm::mat4(1.0f);

    // Translate the scene backward (move camera forward)
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -m_fDistance));

    // Rotate around X-axis (up/down movement)
    view = glm::rotate(view, glm::radians(m_fAngleX), glm::vec3(1.0f, 0.0f, 0.0f));

    // Rotate around Y-axis (left/right movement)
    view = glm::rotate(view, glm::radians(m_fAngleY), glm::vec3(0.0f, 1.0f, 0.0f));

    return view;
}

void TrackballCamera::handleMouseMotion(float deltaX, float deltaY)
{
    float sensitivity = 0.2f;
    m_fAngleY += deltaX * sensitivity; // Rotation gauche/droite
    m_fAngleX += deltaY * sensitivity; // Rotation haut/bas
}
