#pragma once

#include <Utilities/Flags.h>
#include <Utilities/UUID.h>

BEGINNAMESPACE

DECLARE_EXCLUSIVE_FLAGS(MapType, Diffuse, Specular, Ambient, Emissive, Height, Normal, Shininess, Opacity, Displacement, Lightmap, Reflection, Unknown);
DECLARE_EXCLUSIVE_FLAGS(ShadingModel, Phong, Blinn);
DECLARE_EXCLUSIVE_FLAGS(BlendMode, Additive, Default);

struct MaterialSpec {
	static const uint32 MaxMaterialNameLength = 80;

	UUID uuid;

	ansichar name[MaxMaterialNameLength];
	union {
		uint32 flags;
		struct {
			uint32 shadingModel : 4;
			uint32 blendMode : 4;
		};
	};
	float32 opacity, shininess, shininessStrength, refracti, bumbScaling;
	typedef float32 ColorStorage[4];
	ColorStorage diffuseColor, specularColor, ambientColor, emissiveColor, transparentColor, reflectiveColor;

	/*
	aiTextureType_NONE = 0x0,
	aiTextureType_DIFFUSE = 0x1,
	aiTextureType_SPECULAR = 0x2,
	aiTextureType_AMBIENT = 0x3,
	aiTextureType_EMISSIVE = 0x4,
	aiTextureType_HEIGHT = 0x5,
	aiTextureType_NORMALS = 0x6,
	aiTextureType_SHININESS = 0x7,
	aiTextureType_OPACITY = 0x8,
	aiTextureType_DISPLACEMENT = 0x9,
	aiTextureType_LIGHTMAP = 0xA,
	aiTextureType_REFLECTION = 0xB,
	aiTextureType_UNKNOWN = 0xC,
	*/

	union {
		uint8 numberOfMaps[11];
		struct {
			uint8 diffuseMap;
			uint8 specularMap;
			uint8 ambientMap;
			uint8 emissiveMap;
			uint8 heightMap;
			uint8 normalMap;
			uint8 shininessMap;
			uint8 opacityMap;
			uint8 displacementMap;
			uint8 lightMap;
			uint8 reflectionMap;
		};
	};

	union {
		UUID* textureRefs;
		UIntOfPtrSize __textureRefLocation;
	};
	static MaterialSpec* FromBuffer(const Byte* buffer);
	static uint32 MemSize(const MaterialSpec* spec);
	static void ToBuffer(const MaterialSpec* res, Byte* buffer);
};


ENDNAMESPACE