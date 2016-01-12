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
	glm::mat4x4 getPreviousViewProjection() const;
    glm::ivec2 getViewportSize() const;
	glm::vec2 getClippingPlanes() const;

	inline const GameObject* getGameObject() const { return m_BelongTo; }

    void setViewportSize(glm::uvec2 vec);
    void setVerticalFieldOfView(float _fovv);
private:
    GameObject* m_BelongTo;
	glm::mat4x4 m_PreviousViewProjection;
    Byte m_CameraStorage[256];
};

ENDNAMESPACE
