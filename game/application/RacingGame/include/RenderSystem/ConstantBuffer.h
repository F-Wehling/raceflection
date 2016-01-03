#pragma once

#include "glm/glm.hpp"

BEGINNAMESPACE

//
/// Constant Buffer (-> same as in the shader code)

struct ViewProjectionMatrices {
	glm::mat4 m4_ViewProjection;
	glm::mat4 m4_Projection;
	glm::mat4 m4_ProjectionI;
	glm::mat4 m4_View;
	glm::mat4 m4_ViewIT;
	glm::vec3 v3_EyePos;
	float32 dummy;
	glm::vec2 v2_ClippingPlanes;
	glm::ivec2 iv2_ViewportSize;
};

struct ModelMatrices {
	glm::mat4 m4_Model;
	glm::mat4 m4_ModelView;
	glm::mat4 m4_ModelViewProjection;
	glm::mat4 m4_ModelIT;
};

//
/// !IMPORTANT
/// To modify the light structure, you have also to modify the 
/// shader code constants 
struct Light {
	static const uint32 MaxLights = 100; //MUST MATCH SHADER CONSTANT
	uint32 type;
	glm::vec3 v3_position;
	glm::vec3 v3_dir;
	glm::vec3 v3_diffuse;
	glm::vec3 v3_specular;
	glm::vec3 v3_ambient;
	float32 f_AttenuationCst;
	float32 f_AttenuationLin;
	float32 f_AttenuationSq;
	float32 f_AngleInnerCone;
	float32 f_AngleOuterCone;
	float32 f_Padding[3];
};

ENDNAMESPACE