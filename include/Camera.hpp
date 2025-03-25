#pragma once
#include "FreeflyCamera.hpp"
#include "TrackballCamera.hpp"

enum class CameraMode {
    Trackball,
    Freefly
};

class Camera {
private:
    TrackballCamera m_Trackball;
    FreeflyCamera   m_Freefly;
    CameraMode      m_Mode;

public:
    Camera();

    void       setMode(CameraMode mode);
    CameraMode getMode() const { return m_Mode; }
};