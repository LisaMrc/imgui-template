#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>


class FreeflyCamera {
    private:
        glm::vec3 m_Position;
        float m_fPhi;
        float m_fTheta;
    
        glm::vec3 m_FrontVector;
        glm::vec3 m_LeftVector;
        glm::vec3 m_UpVector;

        glm::vec3 m_PiecePosition;

        void computeDirectionVectors();
    
    public:
        FreeflyCamera();
        void attachToPiece(glm::vec3 piecePosition);
        // void moveLeft(float t);
        // void moveFront(float t);
        void rotateLeft(float degrees);
        void rotateUp(float degrees);
        glm::mat4 getViewMatrix() const;
        
    };