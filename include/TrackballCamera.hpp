#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

class TrackballCamera {
private:
    float m_fDistance; // distance par rapport au centre de la scène
    float m_fAngleX;   // angle effectuée par la caméra autour de l'axe x de la scène (rotation vers le haut ou vers le bas)
    float m_fAngleY;   // angle effectuée par la caméra autour de l'axe y de la scène (rotation vers la gauche ou vers la droite)

public:
    explicit TrackballCamera(float m_fDistance0 = 5.f, float m_fAngleX0 = 0.f, float m_fAngleY0 = 0.f)
        : m_fDistance(m_fDistance0), m_fAngleX(m_fAngleX0), m_fAngleY(m_fAngleY0) {};

    void moveFront(float delta);
    void rotateLeft(float degrees);
    void rotateUp(float degrees);

    glm::mat4 getViewMatrix() const;

    void handleMouseMotion(float deltaX, float deltaY);
};