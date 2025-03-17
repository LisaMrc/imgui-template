#include "../include/FreeflyCamera.hpp"
#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include "imgui.h"

constexpr float PI = 3.14159265358979323846f;

void FreeflyCamera::computeDirectionVectors()
{
    m_FrontVector = glm::vec3(cos(m_fTheta) * sin(m_fPhi), sin(m_fTheta), cos(m_fTheta) * cos(m_fPhi));
    m_LeftVector  = glm::vec3(sin(m_fPhi + PI / 2), 0, cos(m_fPhi + PI / 2));
    m_UpVector    = glm::cross(m_FrontVector, m_LeftVector);
}

FreeflyCamera::FreeflyCamera()
    : m_Position(0.0f, 0.0f, 3.0f), m_fPhi(PI / 2.0f), m_fTheta(0)
{
    computeDirectionVectors();
}

void FreeflyCamera::attachToPiece(glm::vec3 piecePosition)
{
    m_PiecePosition = piecePosition;
    m_Position      = piecePosition + glm::vec3(0.0f, 1.5f, 0.0f); // Caméra au sommet de la pièce
    // m_Mode = CameraMode::PieceView;
}

// void FreeflyCamera::moveLeft(float t)
// {
//     m_Position += t * m_LeftVector;
// }

// void FreeflyCamera::moveFront(float t)
// {
//     m_Position += t * m_FrontVector;
// }

void FreeflyCamera::rotateLeft(float degrees)
{
    float radians = glm::radians(degrees);
    m_fPhi += radians;
    computeDirectionVectors();
}

void FreeflyCamera::rotateUp(float degrees)
{
    float radians = glm::radians(degrees);
    m_fTheta += radians;
    computeDirectionVectors();
}

glm::mat4 FreeflyCamera::getViewMatrix() const
{
    return glm::lookAt(m_Position, m_Position + m_FrontVector, m_UpVector);
}