#pragma once

#include <glm/glm.hpp>

BEGINNAMESPACE

class GameObject;

class Camera {
public:
public:
    Camera();

    void attachToObject(GameObject* go){ m_BelongTo = go; }
public:
    void update();
    glm::mat4x4 getViewMatrix() const;
    glm::mat4x4 getProjectionMatrix() const;
    glm::uvec2 getViewportSize() const;

    void setViewportSize(glm::uvec2 vec);
private:
    GameObject* m_BelongTo;
    Byte m_CameraStorage[256];
};

ENDNAMESPACE
