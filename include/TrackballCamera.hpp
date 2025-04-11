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
    float m_fAngleX;
    float m_fAngleY;
    float m_fDistance;

    glm::vec3 m_Target;
};