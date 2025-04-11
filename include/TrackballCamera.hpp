#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#pragma once

class TrackballCamera {
public:
    TrackballCamera();

    void moveFront(float delta);
    void rotateLeft(float degrees);
    void rotateUp(float degrees);
    void handleMouseMotion(float deltaX, float deltaY);

    glm::mat4 getViewMatrix() const;

private:
    float m_fAngleX;   // Pitch (vertical)
    float m_fAngleY;   // Yaw (horizontal)
    float m_fDistance; // Distance à la cible (zoom)

    glm::vec3 m_Target; // Point regardé
};