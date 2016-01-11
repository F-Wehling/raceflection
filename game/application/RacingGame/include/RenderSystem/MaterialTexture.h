#pragma once

#include <RenderSystem/RenderHandles.h>

BEGINNAMESPACE

struct Material;
struct MaterialSpec;

struct MaterialTexture {
	enum {
		Diffuse = 0,
		Specular,
		Ambient,
		Emissive,
		Height,
		Normal,
		Shininess,
		Opacity,
		Displacement,
		Light,
		Reflection,
		count
	};
	const MaterialSpec* m_CreatedFrom;
	const Material* m_GPURepresentation;
	uint32 m_GPUId;

	TextureHandle m_Textures[count];	
};

ENDNAMESPACE