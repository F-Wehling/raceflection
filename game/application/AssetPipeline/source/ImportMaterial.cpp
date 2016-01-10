#include <ImportMaterial.h>
#include <ImportTexture.h>

#include <assimp/material.h>
#include <assimp/texture.h>

#include <Logging/Logging.h>

#include <Utilities/Assert.h>

#include "Filesystem.h"

#include <map>

BEGINNAMESPACE

typedef std::map<UUID, MaterialSpec*> MaterialList_t;
MaterialList_t g_MaterialList;

namespace Importer {
	UUID materialByName(const ansichar * name)
	{
		for (auto mat : g_MaterialList) {
			if (strncmp(mat.second->name, name, MaterialSpec::MaxMaterialNameLength) == 0) return mat.first;
		}
		UUID zero = { 0,0 };
		return zero;
	}

	UUID materialByIndex(uint32 index)
	{
		return UUID();
	}

	uint32 FromAiTextureType(aiTextureType type) {
		switch (type) {
		case aiTextureType_DIFFUSE: return MapType::Diffuse;
		case aiTextureType_SPECULAR: return MapType::Specular;
		case aiTextureType_AMBIENT: return MapType::Ambient;
		case aiTextureType_EMISSIVE: return MapType::Emissive;
		case aiTextureType_HEIGHT: return MapType::Height;
		case aiTextureType_NORMALS: return MapType::Normal;
		case aiTextureType_SHININESS: return MapType::Shininess;
		case aiTextureType_OPACITY: return MapType::Opacity;
		case aiTextureType_DISPLACEMENT: return MapType::Displacement;
		case aiTextureType_LIGHTMAP: return MapType::Lightmap;
		case aiTextureType_REFLECTION: return MapType::Reflection;
		case aiTextureType_UNKNOWN: return MapType::Unknown;
		default: return 0;
		}
		return 0;
	}

	uint32 FromAiShadingModel(aiShadingMode model) {
		switch (model) {
		case aiShadingMode_Phong: return ShadingModel::Phong;
		default:
			return ShadingModel::Blinn;
		}
	}

	uint32 FromAiBlendMode(aiBlendMode mode) {
		switch(mode) {
		case aiBlendMode_Additive: return BlendMode::Additive;
		default: return BlendMode::Default;
		}
	}

	void parseMaterial(const aiMaterial* aiMaterial, MaterialSpec* material) {
		
		//
		/// read material name
		aiString materialName;
		aiMaterial->Get(AI_MATKEY_NAME, materialName);
		ASSERT(materialName.length < MaterialSpec::MaxMaterialNameLength, "A material-name should be at most %d Bytes", MaterialSpec::MaxMaterialNameLength);
		std::copy(materialName.data, materialName.data + MaterialSpec::MaxMaterialNameLength, material->name);

		material->uuid = generateUUID();
		
		//
		/// get some overall parameter
		material->flags = 0;
		int32 shading_model = aiShadingMode_Blinn; //we only use blinn and phong model
		aiMaterial->Get(AI_MATKEY_SHADING_MODEL, shading_model);
		ASSERT(shading_model == aiShadingMode_Blinn || shading_model == aiShadingMode_Phong, "Unsupported shading mode!");
		material->shadingModel = FromAiShadingModel(aiShadingMode(shading_model));

		int32 blendfunc = aiBlendMode::aiBlendMode_Default;
		aiMaterial->Get(AI_MATKEY_BLEND_FUNC, blendfunc);
		material->blendMode = FromAiBlendMode(aiBlendMode(blendfunc));

		aiMaterial->Get(AI_MATKEY_OPACITY, material->opacity);
		aiMaterial->Get(AI_MATKEY_SHININESS, material->shininess);
		aiMaterial->Get(AI_MATKEY_SHININESS_STRENGTH, material->shininessStrength);
		aiMaterial->Get(AI_MATKEY_REFRACTI, material->refracti);
		aiMaterial->Get(AI_MATKEY_BUMPSCALING, material->bumbScaling);
		/*
		int32 twoSided = 0;
		aiMaterial->Get(AI_MATKEY_TWOSIDED, twoSided);
		material->header.twoSided = twoSided != 0 ? 1 : 0;
		int32 wireframe = 0;
		aiMaterial->Get(AI_MATKEY_ENABLE_WIREFRAME, wireframe);
		material->header.wireframe = wireframe != 0 ? 1 : 0;
		*/

		aiMaterial->Get(AI_MATKEY_COLOR_DIFFUSE,*(aiColor4D*)material->diffuseColor);
		aiMaterial->Get(AI_MATKEY_COLOR_SPECULAR, *(aiColor4D*)material->specularColor);
		aiMaterial->Get(AI_MATKEY_COLOR_AMBIENT, *(aiColor4D*)material->ambientColor);
		aiMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, *(aiColor4D*)material->emissiveColor);
		aiMaterial->Get(AI_MATKEY_COLOR_TRANSPARENT, *(aiColor4D*)material->transparentColor);
		aiMaterial->Get(AI_MATKEY_COLOR_REFLECTIVE, *(aiColor4D*)material->reflectiveColor);
		///
		//
		UUID* textureId = material->textureRefs;
		std::fill(material->numberOfMaps, material->numberOfMaps + aiTextureType_UNKNOWN, 0);
		for (uint32 textureType = 0; textureType < aiTextureType_UNKNOWN; ++textureType) {			
			aiTextureType aiTex = aiTextureType(textureType + 1);
			material->numberOfMaps[textureType] = uint16(aiMaterial->GetTextureCount(aiTex)); //< store the number of textures for this type

			for (uint32 texIdx = 0; texIdx < aiMaterial->GetTextureCount(aiTex); ++texIdx) {
				//as textures aren't contained inside the scene files we load each texture from file
				aiString textureFile;
				aiTextureMapping mapping = aiTextureMapping_UV;
				uint32 uvIndex = 0;
				float32 blend = 0;
				aiTextureOp op = aiTextureOp_Add;
				aiTextureMapMode mapMode[2] = { aiTextureMapMode_Wrap, aiTextureMapMode_Wrap };
				aiMaterial->GetTexture(aiTex, texIdx, &textureFile, &mapping, &uvIndex, &blend, &op, mapMode);
				//get texture reference
				ASSERT(mapping == aiTextureMapping_UV, "The mapping should rely on UVW-Coordinates");
			
				filesys::path p = filesys::stem(textureFile.C_Str());
				UUID id = getTexture(p);
				*textureId++ = id;
			}
		}

		g_MaterialList[material->uuid] = material;

	}

	MaterialSpec* initializeMaterial(const aiMaterial* material) {
		union {
			Byte* buffer;
			MaterialSpec* spec;
		};
		uint32 numTextures = 0;
		for (uint32 textureType = 0; textureType < aiTextureType_UNKNOWN; ++textureType) {
			aiTextureType aiTex = aiTextureType(textureType + 1);
			numTextures += material->GetTextureCount(aiTex);
		}

		uint32 size = numTextures * sizeof(UUID) + sizeof(MaterialSpec);
		buffer = new Byte[size];
		std::memset(buffer, 0, size);
		spec->__textureRefLocation = UIntOfPtrSize(buffer) + sizeof(MaterialSpec);
		return spec;
	}

	Materials materialAllFromScene(const aiScene * scene)
	{
		Materials materials(scene->mNumMaterials);
		for (uint32 matIdx = 0; matIdx < scene->mNumMaterials; ++matIdx) {
			materials[matIdx] = initializeMaterial(scene->mMaterials[matIdx]);
			parseMaterial(scene->mMaterials[matIdx], materials[matIdx]);
		}
		return materials;
	}
	Materials materialAllFromFile(const filesys::path & file)
	{
		return Materials();
	}
	/*
	void processTextureMap(const aiMaterial* aiMaterial, aiTextureType type, uint32 index) {
		aiString path;
		aiTextureMapping mapping = aiTextureMapping_UV;
		uint32 uvIndex = 0;
		float32 blend = 0;
		aiTextureOp op = aiTextureOp_Add;
		aiTextureMapMode mapMode = aiTextureMapMode_Wrap;
		if (AI_SUCCESS != aiMaterial->GetTexture(type, index, &path, &mapping, &uvIndex, &blend, &op)) {
			LOG_WARNING(General, "Can't get the Texture-informations (channel: %d)(textureidx: %d)", type, index);
		}
	}

	Material * material_load(const path & path)
	{
		LOG_ERROR(General, "Load materials from file not implemented. Store as scene instead");
		return nullptr;
	}

	void material_write(FILE* file, Material * mesh)
	{
	}

	void material_setOffsets(Material * material)
	{
	}

	void material_process(const aiMaterial* aiMaterial, Package& package) {
		Material mat;

		aiString materialName;
		aiMaterial->Get(AI_MATKEY_NAME, materialName);

		int32 wireframe = 0;
		aiMaterial->Get(AI_MATKEY_ENABLE_WIREFRAME, wireframe);

		int32 twosided = 0;
		aiMaterial->Get(AI_MATKEY_TWOSIDED, twosided);

		int32 shading_model = aiShadingMode_Blinn;
		aiMaterial->Get(AI_MATKEY_SHADING_MODEL, shading_model);

		int32 blendfunc = aiBlendMode::aiBlendMode_Default;
		aiMaterial->Get(AI_MATKEY_BLEND_FUNC, blendfunc);

		float32 opacity = 1.0;
		aiMaterial->Get(AI_MATKEY_OPACITY, opacity);

		float32 shininess = 0.0;
		aiMaterial->Get(AI_MATKEY_SHININESS, shininess);

		float32 shininess_strength = 1.0;
		aiMaterial->Get(AI_MATKEY_REFRACTI, shininess_strength);

		float32 refacti = 1.0;
		aiMaterial->Get(AI_MATKEY_SHININESS_STRENGTH, shininess_strength);

		aiColor3D ambientColor;
		aiColor3D diffuseColor;
		aiColor3D specularColor;
		aiColor3D emissiveColor;
		aiMaterial->Get(AI_MATKEY_COLOR_AMBIENT, ambientColor);
		aiMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);
		aiMaterial->Get(AI_MATKEY_COLOR_SPECULAR, specularColor);
		aiMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, emissiveColor);


		for (uint32 textureType = 0; textureType <= aiTextureType_UNKNOWN; ++textureType) {
			for (uint32 texIdx = 0; texIdx < aiMaterial->GetTextureCount(aiTextureType(textureType)); ++texIdx) {
				processTextureMap(aiMaterial, (aiTextureType)textureType, texIdx);
			}
		}
	}

	uint32 material_getSize(const Material * material)
	{
		return uint32();
	}
	*/
}

ENDNAMESPACE