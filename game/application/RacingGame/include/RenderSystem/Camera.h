#pragma once

#include <glm/glm.hpp>

BEGINNAMESPACE

class GameObject;

class Camera {
    enum CameraType {
        FIXED,
        GENERIC
    };
public:
    Camera(CameraType type = GENERIC);
public:
    void update();
    glm::mat4x4 getViewMatrix() const;
    glm::mat4x4 getPerspectiveMatrix() const;
    glm::uvec2 getViewportSize() const;
private:
    CameraType m_Type;
    GameObject* m_BelongTo;
    Byte m_CameraStorage[256];
};

ENDNAMESPACE
