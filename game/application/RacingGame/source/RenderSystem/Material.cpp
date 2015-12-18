#include "RenderSystem/Material.h"
#include "MaterialSpec.h"
BEGINNAMESPACE

size_type MaterialSizeFromSpecification(const MaterialSpec* spec) {
	

	uint32 numOfMaps = 0;
	for (uint32 i = 0; i < 11; ++i) {
		numOfMaps += spec->numberOfMaps[i];
	}
	return sizeof(Material) + sizeof(TextureHandle) * numOfMaps;
}

Material* CreateMaterialFromSpecification(const MaterialSpec* spec, Material* _material) {
	union {
		Material* material;
		UIntOfPtrSize location;
	};
	material = _material;
	UIntOfPtrSize offset = sizeof(MaterialSpec);
	
	for (uint32 i = 0; i < 11; ++i) {
		material->__textureHandleLocation[i] = location + offset;
		offset += sizeof(TextureHandle) * spec->numberOfMaps[i];
		material->m_NumberOfMaps[i] = spec->numberOfMaps[i];
	}

	std::memcpy(material->m_AmbientColor, spec->ambientColor, sizeof(MaterialSpec::ColorStorage));
	std::memcpy(material->m_DiffuseColor, spec->diffuseColor, sizeof(MaterialSpec::ColorStorage));
	std::memcpy(material->m_EmissiveColor, spec->emissiveColor, sizeof(MaterialSpec::ColorStorage));
	std::memcpy(material->m_SpecularColor, spec->specularColor, sizeof(MaterialSpec::ColorStorage));
	std::memcpy(material->m_TransparentColor, spec->transparentColor, sizeof(MaterialSpec::ColorStorage));
	std::memcpy(material->m_ReflectiveColor, spec->reflectiveColor, sizeof(MaterialSpec::ColorStorage));
	
	material->m_Opacity = spec->opacity;
	material->m_Shininess = spec->shininess;
	material->m_ShininessStrength = spec->shininessStrength;
	material->m_Refracti = spec->refracti;
	material->m_BumbScaling = spec->bumbScaling;

	return material;
}

ENDNAMESPACE