#pragma once

#include <RenderSystem/RenderHandles.h>

BEGINNAMESPACE

struct MaterialSpec;

struct Material {
	MaterialSpec* m_CreatedFrom;

	union {
		union {
			TextureHandle* m_TextureHandles[11];
			UIntOfPtrSize __textureHandleLocation[11];
		};
		struct {
			union {
				TextureHandle* m_DiffuseTextures;
				UIntOfPtrSize __diffuseTextureLocation;
			};

			union {
				TextureHandle* m_SpecularTextures;
				UIntOfPtrSize __specularTextureLocation;
			};

			union {
				TextureHandle* m_AmbientTextures;
				UIntOfPtrSize __ambientTextureLocation;
			};

			union {
				TextureHandle* m_EmissiveTextures;
				UIntOfPtrSize __emissiveTextureLocation;
			};

			union {
				TextureHandle* m_HeightTextures;
				UIntOfPtrSize __heightTextureLocation;
			};

			union {
				TextureHandle* m_NormalTextures;
				UIntOfPtrSize __normalTextureLocation;
			};

			union {
				TextureHandle* m_ShininessTextures;
				UIntOfPtrSize __shininessTextureLocation;
			};

			union {
				TextureHandle* m_OpacityTextures;
				UIntOfPtrSize __opacityTextureLocation;
			};

			union {
				TextureHandle* m_DisplacementTextures;
				UIntOfPtrSize __displacementTextureLocation;
			};

			union {
				TextureHandle* m_LightTextures;
				UIntOfPtrSize __lightTextureLocation;
			};

			union {
				TextureHandle* m_ReflectionTextures;
				UIntOfPtrSize __reflectionTextureLocation;
			};
		};
	};

	union {
		uint8 m_NumberOfMaps[11];
		struct {
			uint8 m_NumberOfDiffuseTextures;
			uint8 m_NumberOfSpecularTextures;
			uint8 m_NumberOfAmbientTextures;
			uint8 m_NumberOfEmissiveTextures;
			uint8 m_NumberOfHeightTextures;
			uint8 m_NumberOfNormalTextures;
			uint8 m_NumberOfShininessTextures;
			uint8 m_NumberOfOpacityTextures;
			uint8 m_NumberOfDisplacementTextures;
			uint8 m_NumberOfLightTextures;
			uint8 m_NumberOfReflectionTextures;
		};
	};

	float32 m_Opacity, m_Shininess, m_ShininessStrength, m_Refracti, m_BumbScaling;
	typedef float32 ColorStorage[4];
	ColorStorage m_DiffuseColor, m_SpecularColor, m_AmbientColor, m_EmissiveColor, m_TransparentColor, m_ReflectiveColor;
	
};

size_type MaterialSizeFromSpecification(const MaterialSpec* spec);
Material* CreateMaterialFromSpecification(const MaterialSpec* spec, Material* _material);

ENDNAMESPACE