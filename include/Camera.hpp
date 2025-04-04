#pragma once
#include "FreeflyCamera.hpp"
#include "TrackballCamera.hpp"
#include "glm/fwd.hpp"

enum class CameraMode {
    Trackball,
    Freefly
};

class Camera {
    private:
        TrackballCamera m_Trackball{200, 45., 90.};
        FreeflyCamera   m_Freefly;
        CameraMode      m_Mode;

        glm::mat4 ProjMatrix;
        glm::mat4 MVMatrix;
        glm::mat4 NormalMatrix;
        glm::mat4 MVP;
    
    public:
        Camera();

        glm::mat4 get_ProjMatrix() { return ProjMatrix;}
        glm::mat4 get_MVMatrix() { return MVMatrix;}
        glm::mat4 get_NormalMatrix() { return NormalMatrix;}
        glm::mat4 get_MVP() { return MVP;}


        void init_mat_proj()
        {
            ProjMatrix   = glm::perspective(glm::radians(70.f), 1.f, 0.1f, 500.f);
            MVMatrix     = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, -500.f));
            NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
            MVP          = ProjMatrix * MVMatrix * m_Trackball.getViewMatrix();
        }
    
        void       setMode(CameraMode mode);
        CameraMode getMode() const { return m_Mode; }
        glm::mat4  getViewMatrix() const;
    
        // Méthodes unifiées pour contrôler la caméra active
        void moveFront(float t);
        void rotateLeft(float degrees);
        void rotateUp(float degrees);
        void handleMouseMotion(float deltaX, float deltaY);
    };

