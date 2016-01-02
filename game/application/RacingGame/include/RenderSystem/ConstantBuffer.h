#pragma once

#include "glm/glm.hpp"

BEGINNAMESPACE

//
/// Constant Buffer (-> same as in the shader code)

struct ViewProjectionMatrices {
	glm::mat4 m4_ViewProjection;
	glm::mat4 m4_Projection;
	glm::mat4 m4_View;
	glm::mat4 m4_ViewIT;
	glm::vec3 v3_EyePos;
	float32 dummy;
};

struct ModelMatrices {
	glm::mat4 m4_Model;
	glm::mat4 m4_ModelView;
	glm::mat4 m4_ModelViewProjection;
	glm::mat4 m4_ModelIT;
};

ENDNAMESPACE