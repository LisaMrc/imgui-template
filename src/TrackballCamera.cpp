#pragma once
#include "../include/TrackballCamera.hpp"
#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>

// void TrackballCamera::computeDirectionVectors()
// {
//     float radX = glm::radians(m_fAngleX);
//     float radY = glm::radians(m_fAngleY);

//     glm::vec3 front;
//     front.x = cos(radX) * sin(radY);
//     front.y = sin(radX);
//     front.z = cos(radX) * cos(radY);
// }

TrackballCamera::TrackballCamera()
    : m_fAngleX(45.0f), m_fAngleY(0.0f), m_fDistance(5.0f), m_Target(glm::vec3(0.0f, 0.0f, 0.0f)) {}

void TrackballCamera::moveFront(float delta)
{
    m_fDistance += delta;

    if (m_fDistance < 1.0f)
        m_fDistance = 1.0f;
    if (m_fDistance > 20.0f)
        m_fDistance = 20.0f;
}

void TrackballCamera::rotateLeft(float degrees)
{
    m_fAngleY += degrees;
}

void TrackballCamera::rotateUp(float degrees)
{
    m_fAngleX += degrees;

    if (m_fAngleX > 30.0f)
        m_fAngleX = 30.0f;
    if (m_fAngleX < -89.0f)
        m_fAngleX = -89.0f;
}

glm::mat4 TrackballCamera::getViewMatrix() const
{
    float camX = m_fDistance * cos(glm::radians(m_fAngleX)) * sin(glm::radians(m_fAngleY));
    float camY = m_fDistance * sin(glm::radians(m_fAngleX));
    float camZ = m_fDistance * cos(glm::radians(m_fAngleX)) * cos(glm::radians(m_fAngleY));

    glm::vec3 position = glm::vec3(camX, camY, camZ) + m_Target;

    return glm::lookAt(position, m_Target, glm::vec3(0.0f, 1.0f, 0.0f));
}

void TrackballCamera::handleMouseMotion(float deltaX, float deltaY)
{
    float sensitivity = 0.2f;
    rotateLeft(deltaX * sensitivity);
    rotateUp(-deltaY * sensitivity);
}